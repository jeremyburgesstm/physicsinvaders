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

#include "GameSetup.h"
#include "EntityCreation.h"

#include "Core/StateMachine/ThreadedStateMachine.h"
#include "ComponentModel/EntityComponentManager.h"
#include "CoreComponents/CameraComponent.h"
#include "CoreComponents/Box2DBodyComponent.h"
#include "CoreComponents/MoveableQuadComponent.h"
#include "Game/Components/TurretYokeComponent.h"
#include "Game/Components/TurretPointerMovementComponent.h"
#include "Game/Components/TurretController.h"
#include "Game/Components/InvaderWaveMover.h"
#include "Game/Components/InvaderWaveManager.h"
#include "Game/Components/Invader.h"
#include "Game/Components/Bullet.h"
#include "Game/GameStates/SetupGameState.h"
#include "Game/GameStates/SetupStageState.h"
#include "Game/GameStates/WaitPausedState.h"
#include "Game/GameStates/PlayState.h"
#include "Game/GameStates/CheckMoreLevelsState.h"
#include "Game/GameStates/EndState.h"
#include "Game/GameContext.h"
#include "Game/Data/InvaderWaveDefinition.h"
#include "Graphics/RendererD3D.h"
#include "Graphics/Texture2DD3D.h"
#include "Graphics/TextureManager.h"
#include "Utility/Helpers.h"



using namespace ComponentModel;
void ShouldBeDataDriven::SetupEntityManager(ComponentModel::EntityComponentManager* entityManager)
{
	// Add all required component types with counts/priorities
	entityManager->AddComponentType<Box2DBodyComponent>(100, 0);
	entityManager->AddComponentType<MoveableQuadComponent>(100, 1);
	entityManager->AddComponentType<CameraComponent>(5, 10);
	entityManager->AddComponentType<TurretPointerMovementComponent>(1, -10);
	entityManager->AddComponentType<TurretYokeComponent>(1, -5);
	entityManager->AddComponentType<TurretController>(1, -5);
	entityManager->AddComponentType<InvaderWaveMover>(1, -5);
	entityManager->AddComponentType<InvaderWaveManager>(1, 0);
	entityManager->AddComponentType<Invader>(50, -4);
	entityManager->AddComponentType<Bullet>(70, -3);
	entityManager->RefreshUpdateLists();
}

void ShouldBeDataDriven::LoadGameTextures(TextureManager* textureManager, const RendererD3D& renderer)
{
	std::string textureNames[] = {"Content/Textures/turret.png,turret",
								"Content/Textures/invader0.png,invader0",
								"Content/Textures/invader1.png,invader1",
								"Content/Textures/invader2.png,invader2",
								"Content/Textures/playerBullet.png,playerBullet",
								"Content/Textures/invaderBullet.png,invaderBullet",
								"Content/Textures/opinvader.png,opinvader"};
	
	for (size_t i = 0; i < Helpers::SizeOfArray(textureNames); ++i)
	{
		Texture2DD3D* texture = new Texture2DD3D();
		texture->LoadTextureFromFile(textureNames[i].substr(0, textureNames[i].find(',')), renderer);
		textureManager->AddTexture(textureNames[i].substr(textureNames[i].find(',') + 1), texture);
	}
}

void ShouldBeDataDriven::SetupStateMachine(ThreadedStateMachine<const GameStateContext*>* stateMachine)
{
	SetupGameState* setupGameState = new SetupGameState();
	SetupStageState* setupStageState = new SetupStageState();
	WaitPausedState* waitPausedPreGameState = new WaitPausedState(2000, "GET READY!");
	WaitPausedState* waitPausedPostGameState = new WaitPausedState(1000, "WAVE COMPLETED!");
	PlayState* playState = new PlayState();
	CheckMoreLevelsState* checkLevelsState = new CheckMoreLevelsState();
	EndState* victoryState = new EndState(true);
	EndState* defeatState = new EndState(false);
	stateMachine->AddState(setupGameState, true);
	stateMachine->AddState(setupStageState);
	stateMachine->AddState(waitPausedPreGameState);
	stateMachine->AddState(waitPausedPostGameState);
	stateMachine->AddState(playState);
	stateMachine->AddState(checkLevelsState);
	stateMachine->AddState(victoryState);
	stateMachine->AddState(defeatState);
	stateMachine->ConnectStates(setupGameState, setupStageState, 0);
	stateMachine->ConnectStates(setupStageState, waitPausedPreGameState, 0);
	stateMachine->ConnectStates(waitPausedPreGameState, playState, 0);
	stateMachine->ConnectStates(playState, checkLevelsState, PlayState::STATE_EXIT_PLAYER_WON);
	stateMachine->ConnectStates(playState, defeatState, PlayState::STATE_EXIT_PLAYER_LOST);
	stateMachine->ConnectStates(checkLevelsState, waitPausedPostGameState, CheckMoreLevelsState::STATE_EXIT_MORE_LEVELS);
	stateMachine->ConnectStates(waitPausedPostGameState, setupStageState, CheckMoreLevelsState::STATE_EXIT_MORE_LEVELS);
	stateMachine->ConnectStates(checkLevelsState, victoryState, CheckMoreLevelsState::STATE_EXIT_LEVELS_DONE);
	stateMachine->ConnectStates(victoryState, setupGameState, 0);
	stateMachine->ConnectStates(defeatState, setupGameState, 0);
}

void ShouldBeDataDriven::SetupGame(const GameContext& context, const StateOfTheGame& state)
{
	std::set<std::string> namesToIgnore;
	namesToIgnore.insert(c_cameraName);
	context.GetComponentManager().ClearAll(&namesToIgnore);

	// Create the turret.
	CreateTurret(context, state);

	// Create our play area.
	CreatePlayArea(context);
}

void ShouldBeDataDriven::SetupStage(unsigned int stage, const GameContext& context)
{		
	InvaderWaveDefinition* invaderDef;
	// Create the invader wave
	switch (stage)
	{
	case 0:
		invaderDef = CreateBasicInvaderWaveDef(0.f);
		break;
	case 1:
		invaderDef = CreateDrapesInvaderWaveDef(0.05f);
		break;
	case 2:
		invaderDef = CreateDrapesInvaderWaveDef(0.2f, true);
		break;
	case 3:
		invaderDef = CreateDiagonalInvaderWaveDef(0.4f);
		break;
	case 4:
		invaderDef = CreateBasicInvaderWaveDef(0.5f, 5);
		break;
	default:
		invaderDef = CreateBasicInvaderWaveDef(1.f);
		break;
	}
	CreateInvaderWave(context, invaderDef);
	delete invaderDef;
}

