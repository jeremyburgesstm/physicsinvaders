/*
 * This source file is part of one of Jeremy Burgess's samples.
 *
 * Copyright (c) 2013 Jeremy Burgess 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// My header
#include "GameWorld.h"

// Box 2D
#include <Box2D/Box2D.h>

// Boost
#include <boost/format.hpp>

// Graphics
#include "Graphics/QuadRendererD3D.h"
#include "Graphics/MoveableTexturedQuad.h"
#include "Graphics/TextureManager.h"

// Utility
#include "Utility/Log.h"

// Core types
#include "Core/GameTime.h"
#include "Core/ITimeSource.h"
#include "Core/StateMachine/ThreadedStateMachine.h"
#include "Input/InputSystem.h"

// Component model.
#include "ComponentModel/EntityComponentManager.h"

// Setup/data
#include "ShouldBeDataDriven/GameSetup.h"
#include "ShouldBeDataDriven/EntityCreation.h"

// Game
#include "GameContext.h"
#include "StateOfTheGame.h"
#include "Messaging/GameMessageHub.h"
#include "GameStates/GameStateContext.h"
#include "Screens/HUDScreen.h"
#include "Rocket/ProgressScreen.h"

// For now allow time debugging.
#define ENABLE_PHYSICS_TIME_DEBUGGING

#ifdef ENABLE_PHYSICS_TIME_DEBUGGING
#include "Debug/DebugGameTimeController.h"
#endif

using namespace ComponentModel;
using namespace Eigen;

GameWorld::GameWorld(const RendererD3D& renderer, HUDScreen& hud, ProgressScreen& victory, ProgressScreen& defeat) :
	m_renderer(renderer)
{
	// Create time.
	m_gameTime = new GameTime();
	
	// Setup the core objects which drive the world.
	m_box2DWorld = new b2World(b2Vec2(0, -9.8f));
	m_quadRenderer = new QuadRendererD3D(m_renderer, "Content/Shaders/TexturedUnlit.fx", 50);

	// Create our message hub.
	m_messageHub = new GameMessageHub(m_box2DWorld);
	
	// Create our manager
	m_entityManager = new EntityComponentManager(80);

	// Create the texture manager.
	m_textureManager = new TextureManager();

	// And our game context.
	m_gameContext = new GameContext(*m_messageHub, *m_box2DWorld, *m_entityManager, renderer, *m_quadRenderer, *m_textureManager);

	// Setup the entity manager
	m_entityManager->SetGameContext(m_gameContext);
	ShouldBeDataDriven::SetupEntityManager(m_entityManager);
	
	// State of the game
	m_stateOfTheGame = new StateOfTheGame(5, 5, *m_messageHub);

	// Create our game state context
	m_gameStateContext = new GameStateContext(*m_gameContext, *m_gameTime, *m_stateOfTheGame, hud, victory, defeat);

	// And create the state machine
	m_stateMachine = new ThreadedStateMachine<const GameStateContext*>();

	// Setup the state machine
	ShouldBeDataDriven::SetupStateMachine(m_stateMachine);

	// Load all game textures
	ShouldBeDataDriven::LoadGameTextures(m_textureManager, m_renderer);

	// Setup our camera
	m_camera = ShouldBeDataDriven::CreateCamera(*m_gameContext);
}

GameWorld::~GameWorld(void)
{
	delete m_stateMachine;
	delete m_entityManager;
	delete m_quadRenderer;
	delete m_stateOfTheGame;
	delete m_gameStateContext;
	delete m_gameContext;
	delete m_messageHub;
	delete m_box2DWorld;
}

void GameWorld::CoreUpdate()
{
	// Update the state machine
	m_stateMachine->CoreUpdate(m_gameStateContext);

	// Update the physics world
	for (int i = 0; i < m_gameTime->GetNumStepsThisFrame(); ++i)
	{
		m_entityManager->PhysicsUpdate(*m_gameTime);
		m_box2DWorld->Step(1.f/60.f, 8, 2);
	}	

	// Do the core update
	m_entityManager->CoreUpdate(*m_gameTime);
}

void GameWorld::RenderUpdate()
{
	m_stateMachine->RenderUpdate(m_gameStateContext);
	m_entityManager->RenderUpdate(*m_gameTime);	
	m_quadRenderer->Render(m_renderer, m_camera);
	m_quadRenderer->ClearRenderList();
}

void GameWorld::SynchroniseRenderData()
{
	// If time debugging enabled, update debug time controller
#ifdef ENABLE_PHYSICS_TIME_DEBUGGING
	DebugGameTimeController::UpdateDebugTimeControl(m_gameTime);
#endif
	// Update gametime
	m_gameTime->FrameStarted();

	m_stateMachine->SynchroniseRenderData(m_gameStateContext);
	m_entityManager->SynchroniseRenderData();
}


