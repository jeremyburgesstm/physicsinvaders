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

#pragma once

// Forward declarations
class b2World;
class RendererD3D;
class QuadRendererD3D;
class ICamera;
class GameTime;
class GameMessageHub;
class TextureManager;
class GameContext;
class GameStateContext;
class StateOfTheGame;
class HUDScreen;
class ProgressScreen;
template<typename UpdateArgType>
class ThreadedStateMachine;
namespace ComponentModel
{
	class EntityComponentManager;
};

// Boost inheritance
#include <boost/noncopyable.hpp>

/**
 * \class Gameworld
 *
 * The Game World. Owns the Physics sim, the entity lists, and the 
 * component pools for when we eventually get to that. 
 * 
 * At this stage, just owns a Box 2D sim, and runs it.
 *
 * @author Jeremy Burgess
 */
class GameWorld : public boost::noncopyable
{
public:
	GameWorld(const RendererD3D& renderer, HUDScreen& hud, ProgressScreen& victory, ProgressScreen& defeat);
	~GameWorld(void);

	/// \name Update methods
	/// Note: We mirror the component update structure here so that
	///       it's easier to kick off updates in the correct manner. 
	/// @{		
		/// Core update should do the physics update and any
		/// gameplay related logic.
		void CoreUpdate();

		/// Render update should do any draw related logic.
		void RenderUpdate();

		/// Synchronise render data should be used to push data between
		/// rendering specific structures and core update specific structures.
		void SynchroniseRenderData();
	/// @}

private:
	// Our Box 2D World
	b2World* m_box2DWorld;

	// Our Quad Renderer - should probably be wrapped in a platform independant quad renderer,
	// but for now this will do.
	QuadRendererD3D* m_quadRenderer;

	// The D3D renderer. Needed to render anything complicated.
	const RendererD3D& m_renderer;

	// Entity component manager.
	ComponentModel::EntityComponentManager* m_entityManager;

	// The camera for this game
	ICamera* m_camera;

	// The gametime for this game
	GameTime* m_gameTime;

	// The message hub for this game.
	GameMessageHub* m_messageHub;

	// The texture manager for this game.
	TextureManager* m_textureManager;

	// The game context for this game.
	GameContext* m_gameContext;
	
	// Game state context
	GameStateContext* m_gameStateContext;

	// State machine for driving the game.
	ThreadedStateMachine<const GameStateContext*>* m_stateMachine;

	// Current state of the game
	StateOfTheGame* m_stateOfTheGame;
};

