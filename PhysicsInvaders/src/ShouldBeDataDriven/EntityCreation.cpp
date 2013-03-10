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

#include "EntityCreation.h"

#include "Core/RunInformation.h"
#include "ComponentModel/Entity.h"
#include "ComponentModel/EntityComponentManager.h"
#include "CoreComponents/CameraComponent.h"
#include "CoreComponents/Box2DBodyComponent.h"
#include "CoreComponents/MoveableQuadComponent.h"
#include "Graphics/MoveableTexturedQuad.h"
#include "Graphics/TextureManager.h"
#include "Game/GameContext.h"
#include "Game/StateOfTheGame.h"
#include "Game/Components/TurretYokeComponent.h"
#include "Game/Components/TurretPointerMovementComponent.h"
#include "Game/Components/TurretController.h"
#include "Game/Components/InvaderWaveMover.h"
#include "Game/Components/InvaderWaveManager.h"
#include "Game/Components/Invader.h"
#include "Game/Components/Bullet.h"
#include "Game/Data/InvaderWaveDefinition.h"
#include "Game/Physics/GamePhysicsConstants.h"
#include <Box2D/Box2D.h>

using namespace ComponentModel;

static const char c_leftWallName[] = "LeftWall";
static const char c_rightWallName[] = "RightWall";

ICamera* ShouldBeDataDriven::CreateCamera(const GameContext& context)
{
	// Create the camera
	Entity* testEntity = context.GetComponentManager().GetFreeEntity();
	testEntity->SetName(c_cameraName);
	CameraComponent* camera = context.GetComponentManager().AddComponent<CameraComponent>(testEntity);
	const RunInformation::ScreenDims& screenDims = RunInformation::GetScreenDimensions();
	float height = 600;
	float width = height * (16.f / 9.f);
	float intendedRatio = width / height;
	float minY = 0;
	if (intendedRatio < screenDims.GetScreenAspectRatio())
	{
		width *= screenDims.GetScreenAspectRatio() / intendedRatio;
	}
	else if (intendedRatio > screenDims.GetScreenAspectRatio())
	{
		height *= intendedRatio / screenDims.GetScreenAspectRatio();
		minY += ((screenDims.Height - height) / 2);
	}
	camera->SetOrthographicRect(Rect<float>(-width / 2, width / 2, minY, height + minY));
	camera->SetCameraType(ICamera::CAMERA_TYPE_ORTHOGRAPHIC);
	testEntity->SetPosition(Eigen::Vector3f(0, 0, 5.f));
	testEntity->SetOrientation(Eigen::Quaternionf(Eigen::AngleAxisf((float)M_PI, Eigen::Vector3f::UnitY())));

	// Return the camera
	return camera;
}

void ShouldBeDataDriven::CreateTurret(const GameContext& context, const StateOfTheGame& state)
{
	// Create a new quad for the purposes of this.
	MoveableTexturedQuad* testQuad = new MoveableTexturedQuad(context.GetTextureManager().GetTexture("turret"));
	testQuad->SetSizeMode(MoveableTexturedQuad::SIZE_ACTUAL_SIZE);
	testQuad->SetSizeScale(Eigen::Vector2f(64, 64));
	testQuad->SetPositionOffset(Eigen::Vector2f(-0.5f, -0.5f));
	
	// Create our turret body.
	b2BodyDef def;
	def.linearDamping = 0.5f;
	def.type = b2_dynamicBody;
	def.position.Set(0, 32 / BOX2D_SCALE_FACTOR);
	b2FixtureDef fixDef;
	b2PolygonShape shape;
	shape.SetAsBox(32 / BOX2D_SCALE_FACTOR, 32 / BOX2D_SCALE_FACTOR);
	fixDef.shape = &shape;
	fixDef.restitution = 0.1f;
	fixDef.friction = 0.8f;
	fixDef.filter = GamePhysicsConstants::GetPlayerUnitFilter();
	b2Body* testBody = context.GetBox2DWorld().CreateBody(&def);
	testBody->CreateFixture(&fixDef);

	// Aaaand connect it to some components to make it work.
	Entity* testPhysicsEntity = context.GetComponentManager().GetFreeEntity();
	Box2DBodyComponent* bodyComponent = context.GetComponentManager().AddComponent<Box2DBodyComponent>(testPhysicsEntity);
	bodyComponent->SetBody(testBody);
	MoveableQuadComponent* quadComponent = context.GetComponentManager().AddComponent<MoveableQuadComponent>(testPhysicsEntity);
	quadComponent->SetQuad(testQuad);
	quadComponent->SetRenderer(&context.GetQuadRenderer());
	TurretController* turretController = context.GetComponentManager().AddComponent<TurretController>(testPhysicsEntity);
	turretController->SetNumLives(state.GetTotalLives());
	turretController->SetTimeToRemainInvulnerableAfterDeath(5);
	TurretYokeComponent* tyc = context.GetComponentManager().AddComponent<TurretYokeComponent>(testPhysicsEntity);
	tyc->SetMaxSpeed(15);
	tyc->SetFireDelay(500);
	context.GetComponentManager().AddComponent<TurretPointerMovementComponent>(testPhysicsEntity);
}

void ShouldBeDataDriven::CreatePlayArea(const GameContext& context)
{
	// Create the play area
	// Floor
	{
		b2BodyDef def;
		def.type = b2_staticBody;
		def.position.Set(0, 0);
		b2FixtureDef fixDef;
		b2PolygonShape shape; 
		shape.SetAsBox(2000 / BOX2D_SCALE_FACTOR, 1 / BOX2D_SCALE_FACTOR);
		fixDef.shape = &shape;
		fixDef.restitution = 0.0f;
		fixDef.friction = 0.7f;
		fixDef.density = 1.f;
		fixDef.filter = GamePhysicsConstants::GetFloorFilter();
		b2Body* floor = context.GetBox2DWorld().CreateBody(&def);
		floor->CreateFixture(&fixDef);

		// Create an entity and attach
		Entity* floorEntity = context.GetComponentManager().GetFreeEntity();
		Box2DBodyComponent* bodyComponent = context.GetComponentManager().AddComponent<Box2DBodyComponent>(floorEntity);
		bodyComponent->SetBody(floor);
	}

	Box2DBodyComponent* leftWall; Box2DBodyComponent* rightWall;

	// Walls
	{
		b2BodyDef def;
		def.type = b2_staticBody;
		def.position.Set(-480 / BOX2D_SCALE_FACTOR, 0);
		b2FixtureDef fixDef;
		b2PolygonShape shape; 
		shape.SetAsBox(1 / BOX2D_SCALE_FACTOR, 1000 / BOX2D_SCALE_FACTOR);
		fixDef.shape = &shape;
		fixDef.restitution = 0.5f;
		fixDef.friction = 0.0f;
		fixDef.density = 1.f;
		b2Body* wall = context.GetBox2DWorld().CreateBody(&def);
		wall->CreateFixture(&fixDef);
		
		// Create an entity and attach
		Entity* wallEntity = context.GetComponentManager().GetFreeEntity();
		wallEntity->SetName(c_rightWallName);
		Box2DBodyComponent* bodyComponent = context.GetComponentManager().AddComponent<Box2DBodyComponent>(wallEntity);
		rightWall = bodyComponent;
		bodyComponent->SetBody(wall);

		def.position.Set(480 / BOX2D_SCALE_FACTOR, 0);
		wall = context.GetBox2DWorld().CreateBody(&def);
		wall->CreateFixture(&fixDef);
		
		// Create an entity and attach
		wallEntity = context.GetComponentManager().GetFreeEntity();
		wallEntity->SetName(c_leftWallName);
		bodyComponent = context.GetComponentManager().AddComponent<Box2DBodyComponent>(wallEntity);
		leftWall = bodyComponent;
		bodyComponent->SetBody(wall);
	}
}

InvaderWaveDefinition* ShouldBeDataDriven::CreateBasicInvaderWaveDef(float difficulty, int numRows)
{
	InvaderWaveDefinition* newDef = new InvaderWaveDefinition();
	newDef->m_difficulty = difficulty;
	for (int row = 0; row < numRows; ++row)
	{
		for (int col = 0; col < 10; ++col)
		{
			InvaderDefinition invDef;
			invDef.m_id = row * 10 + col;
			invDef.m_xOffset = (col - 4.5f) * 60.f;
			invDef.m_yOffset = (row + 1) * -60.f;
			invDef.m_invaderType = row % 3;
			if (row == 0)
			{
				newDef->m_invaderRootConnections.push_back(InvaderRootConnectionDefinition(invDef.m_id, invDef.m_xOffset));
			}
			else
			{
				newDef->m_invaderConnections.push_back(InvaderConnectionDefinition(invDef.m_id, invDef.m_id - 10));
			}
			newDef->m_invaders.push_back(invDef);
		}
	}
	return newDef;
}

InvaderWaveDefinition* ShouldBeDataDriven::CreateDrapesInvaderWaveDef(float difficulty, bool hard)
{
	InvaderWaveDefinition* newDef = new InvaderWaveDefinition();
	newDef->m_difficulty = difficulty;
	int colCount = 11;
	float colOffset = (colCount - 1.f) / 2.f;
	int joinCol = (colCount - 1) / 2;
	int rootJoinInvaderType = hard ? 0 : 2;
	int daisyJoinInvaderType = 1;
	int hangingInvaderType = hard ? 2 : 0;

	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < colCount; ++col)
		{
			InvaderDefinition invDef;
			invDef.m_id = row * colCount + col;
			invDef.m_xOffset = (col - colOffset) * 60.f;
			invDef.m_yOffset = (row + 1) * -60.f;
			if (col % joinCol == 0 && row == 0) { invDef.m_invaderType = rootJoinInvaderType; }
			else if (col % joinCol == 0) { invDef.m_invaderType = daisyJoinInvaderType; }
			else { invDef.m_invaderType = hangingInvaderType; }
			if (row == 0 && col % joinCol == 0)
			{
				newDef->m_invaderRootConnections.push_back(InvaderRootConnectionDefinition(invDef.m_id, invDef.m_xOffset));
			}
			else if (col % joinCol == 0)
			{
				newDef->m_invaderConnections.push_back(InvaderConnectionDefinition(invDef.m_id, invDef.m_id - colCount));
			}
			if (col != 0)
			{
				newDef->m_invaderConnections.push_back(InvaderConnectionDefinition(invDef.m_id, invDef.m_id - 1));
			}
			newDef->m_invaders.push_back(invDef);
		}
	}
	return newDef;
}

InvaderWaveDefinition* ShouldBeDataDriven::CreateDiagonalInvaderWaveDef(float difficulty)
{
	InvaderWaveDefinition* newDef = new InvaderWaveDefinition();
	newDef->m_difficulty = difficulty;
	int colCount = 11;
	float colOffset = (colCount - 1.f) / 2.f;
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 11 - row; ++col)
		{
			InvaderDefinition invDef;
			invDef.m_id = row * colCount + col;
			invDef.m_xOffset = (col - colOffset) * 60.f + row * 30;
			invDef.m_yOffset = (row + 1) * -60.f;
			invDef.m_invaderType = (row + col) % 3;
			if (row == 0)
			{
				newDef->m_invaderRootConnections.push_back(InvaderRootConnectionDefinition(invDef.m_id, invDef.m_xOffset - 30));
				newDef->m_invaderRootConnections.push_back(InvaderRootConnectionDefinition(invDef.m_id, invDef.m_xOffset + 30));
			}
			else
			{
				newDef->m_invaderConnections.push_back(InvaderConnectionDefinition(invDef.m_id, (row - 1) * colCount + col));
				newDef->m_invaderConnections.push_back(InvaderConnectionDefinition(invDef.m_id, (row - 1) * colCount + col + 1));
			}
			newDef->m_invaders.push_back(invDef);
		}
	}
	return newDef;
}

void ShouldBeDataDriven::CreateInvaderWave(
	const GameContext& context, 
	InvaderWaveDefinition* def)
{			
	// Cache component manager for later
	ComponentModel::EntityComponentManager& componentManager = context.GetComponentManager();

	// Create our invader controller.
	b2BodyDef bodyDef;
	bodyDef.linearDamping = 0.5f;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(160 / BOX2D_SCALE_FACTOR, 600 / BOX2D_SCALE_FACTOR);
	bodyDef.gravityScale = 0;
	b2FixtureDef fixDef;
	b2PolygonShape shape;
	shape.SetAsBox(450 / BOX2D_SCALE_FACTOR, 16 / BOX2D_SCALE_FACTOR);
	fixDef.shape = &shape;
	fixDef.restitution = 0.1f;
	fixDef.friction = 0.4f;
	fixDef.filter = GamePhysicsConstants::GetInvaderWaveFilter();
	b2Body* testBody = context.GetBox2DWorld().CreateBody(&bodyDef);
	testBody->CreateFixture(&fixDef);

	// Aaaand connect it to some components to make it work.
	Entity* testPhysicsEntity = componentManager.GetFreeEntity();
	Box2DBodyComponent* bodyComponent = componentManager.AddComponent<Box2DBodyComponent>(testPhysicsEntity);
	bodyComponent->SetBody(testBody);

	InvaderWaveMover* mover = componentManager.AddComponent<InvaderWaveMover>(testPhysicsEntity);
	mover->SetWalls(componentManager.FindEntityByName(c_leftWallName)->GetComponentByTypeFast<Box2DBodyComponent>(), 
		componentManager.FindEntityByName(c_rightWallName)->GetComponentByTypeFast<Box2DBodyComponent>());
	// Add a bunch of movement phases to the mover.
	mover->AddMovementPhase(new InvaderWaveMover::MovementPhaseSidewaysToWall(false));
	mover->AddMovementPhase(new InvaderWaveMover::MovementPhasePause(500));
	mover->AddMovementPhase(new InvaderWaveMover::MovementPhaseDescendByDistance(1));
	mover->AddMovementPhase(new InvaderWaveMover::MovementPhaseSidewaysToWall(true));
	mover->AddMovementPhase(new InvaderWaveMover::MovementPhasePause(500));
	mover->AddMovementPhase(new InvaderWaveMover::MovementPhaseDescendByDistance(1));
	InvaderWaveMover::InvaderMoverConfig imc;
	imc.m_maxMoverAcceleration = 0.075f;
	imc.m_minMoverAcceleration = 0.01f;
	imc.m_minMoverMaxSpeed = 5.f;
	imc.m_maxMoverMaxSpeed = 15.f;
	mover->SetConfig(imc);

	// Add an invader manager so we can track progress
	InvaderWaveManager* mgr = context.GetComponentManager().AddComponent<InvaderWaveManager>(testPhysicsEntity);
	InvaderWaveManager::InvaderWaveConfig iwc;
	iwc.m_baseDifficulty = def->m_difficulty;
	iwc.m_difficultyIncrementPerInvaderDeath = 0.01f;
	iwc.m_minFireRate = 0.2f;
	iwc.m_maxFireRate = 3.f;
	mgr->SetConfig(iwc);

	// Create a map for the purposes of building up connections.
	std::map<int, Entity*> invaderMap;

	for (size_t i = 0; i < def->m_invaders.size(); ++i)
	{
		Entity* out = nullptr;
		switch (def->m_invaders[i].m_invaderType)
		{
		case 0:
			out = CreateInvader0(context, bodyComponent, def->m_invaders[i]);
			break;
		case 1:
			out = CreateInvader1(context, bodyComponent, def->m_invaders[i]);
			break;
		case 2:
			out = CreateInvader2(context, bodyComponent, def->m_invaders[i]);
			break;
		}
		invaderMap[def->m_invaders[i].m_id] = out;
		mgr->AddInvader(out);
	}

	b2RopeJointDef jointDef;
	jointDef.localAnchorA = b2Vec2_zero;
	jointDef.bodyB = testBody;
	for (auto rootConnIt = def->m_invaderRootConnections.begin(); 
		rootConnIt != def->m_invaderRootConnections.end(); 
		++rootConnIt)
	{
		InvaderRootConnectionDefinition conn = *rootConnIt;
		b2Body* first = invaderMap[conn.m_invaderID]->GetComponentByTypeFast<Box2DBodyComponent>()->GetBody();
		jointDef.bodyA = first;
		jointDef.localAnchorB = b2Vec2(-conn.m_xConnectionPosition / BOX2D_SCALE_FACTOR, 0);
		jointDef.localAnchorA = b2Vec2(0, 10 / BOX2D_SCALE_FACTOR);
		jointDef.maxLength = abs(jointDef.bodyA->GetPosition().y - jointDef.bodyB->GetPosition().y) - (10 / BOX2D_SCALE_FACTOR);
		context.GetBox2DWorld().CreateJoint(&jointDef);
		Invader* inv = invaderMap[conn.m_invaderID]->GetComponentByTypeFast<Invader>();
		inv->SetConnectedToRoot(true);
	}

	jointDef.localAnchorB = b2Vec2_zero;
	for (auto connIt = def->m_invaderConnections.begin();
		connIt != def->m_invaderConnections.end();
		++connIt)
	{
		InvaderConnectionDefinition conn = *connIt;
		b2Body* first = invaderMap[conn.m_firstID]->GetComponentByTypeFast<Box2DBodyComponent>()->GetBody();
		b2Body* second = invaderMap[conn.m_secondID]->GetComponentByTypeFast<Box2DBodyComponent>()->GetBody();
		jointDef.bodyA = first;
		jointDef.bodyB = second;
		b2Vec2 distVec = jointDef.bodyA->GetPosition() - jointDef.bodyB->GetPosition();
		jointDef.maxLength = distVec.Length() - (20 / BOX2D_SCALE_FACTOR);
		distVec.Normalize();
		jointDef.localAnchorA = (10 / BOX2D_SCALE_FACTOR) * -distVec;
		jointDef.localAnchorB = (10 / BOX2D_SCALE_FACTOR) * distVec;
		context.GetBox2DWorld().CreateJoint(&jointDef);
		Invader* invA = invaderMap[conn.m_firstID]->GetComponentByTypeFast<Invader>();
		Invader* invB = invaderMap[conn.m_secondID]->GetComponentByTypeFast<Invader>();
		invA->AddConnectedEntity(invB);
		invB->AddConnectedEntity(invA);		
	}
}

ComponentModel::Entity* ShouldBeDataDriven::CreateInvader0(const GameContext& context, Box2DBodyComponent* invaderWave, const InvaderDefinition& def)
{
	// Create the invader now we have the texture.
	Entity* invader = CreateInvader(context, invaderWave, context.GetTextureManager().GetTexture("invader0"), def);

	// Add an actual invader.
	Invader* invComp = context.GetComponentManager().AddComponent<Invader>(invader);
	invComp->SetupInvader(Invader::InvaderConfig(30, 10, 1, 0));
	
	// Return the invader's body
	return invader;
}

ComponentModel::Entity* ShouldBeDataDriven::CreateInvader1(const GameContext& context, Box2DBodyComponent* invaderWave, const InvaderDefinition& def)
{
	// Create the invader now we have the texture.
	Entity* invader = CreateInvader(context, invaderWave, context.GetTextureManager().GetTexture("invader1"), def);
	
	// Add an actual invader.
	Invader* invComp = context.GetComponentManager().AddComponent<Invader>(invader);
	invComp->SetupInvader(Invader::InvaderConfig(20, 20, 1, 0));

	// Return the invader's body
	return invader;
}

ComponentModel::Entity* ShouldBeDataDriven::CreateInvader2(const GameContext& context, Box2DBodyComponent* invaderWave, const InvaderDefinition& def)
{
	// Create the invader now we have the texture.
	Entity* invader = CreateInvader(context, invaderWave, context.GetTextureManager().GetTexture("invader2"), def);
	
	// Add an actual invader.
	Invader* invComp = context.GetComponentManager().AddComponent<Invader>(invader);
	invComp->SetupInvader(Invader::InvaderConfig(10, 30, 1, 0));

	// Return the invader's body
	return invader;
}

Entity* ShouldBeDataDriven::CreateInvader(const GameContext& context, Box2DBodyComponent* invaderWave, const ITexture2D* texture, const InvaderDefinition& def)
{
	int invaderSize = 20;

	// Create a new quad for the purposes of this.
	MoveableTexturedQuad* testQuad = new MoveableTexturedQuad(texture);
	testQuad->SetSizeMode(MoveableTexturedQuad::SIZE_ACTUAL_SIZE);
	testQuad->SetPositionOffset(Eigen::Vector2f(-0.5f, -0.25f));
	testQuad->SetSizeScale(Eigen::Vector2f(invaderSize * 2, invaderSize * 4));

	// Create our invader body.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = (invaderWave->GetBody()->GetPosition() + (1 /  BOX2D_SCALE_FACTOR) * b2Vec2(-def.m_xOffset, def.m_yOffset));
	bodyDef.linearDamping = 0.25f;
	b2FixtureDef fixDef;
	b2PolygonShape shape;
	shape.SetAsBox(invaderSize / BOX2D_SCALE_FACTOR, invaderSize / BOX2D_SCALE_FACTOR);
	shape.m_centroid.y = -0.5f; // Move the center of the shape down.
	fixDef.shape = &shape;
	fixDef.restitution = 0.5f;
	fixDef.friction = 0.4f;
	fixDef.density = 1;
	fixDef.filter = GamePhysicsConstants::GetInvaderUnitFilter();
	b2Body* testBody = context.GetBox2DWorld().CreateBody(&bodyDef);
	testBody->CreateFixture(&fixDef);

	// Aaaand connect it to some components to make it work.
	Entity* testPhysicsEntity = context.GetComponentManager().GetFreeEntity();
	Box2DBodyComponent* bodyComponent = context.GetComponentManager().AddComponent<Box2DBodyComponent>(testPhysicsEntity);
	bodyComponent->SetBody(testBody);
	MoveableQuadComponent* quadComponent = context.GetComponentManager().AddComponent<MoveableQuadComponent>(testPhysicsEntity);
	quadComponent->SetQuad(testQuad);
	quadComponent->SetRenderer(&context.GetQuadRenderer());

	return testPhysicsEntity;
}

Entity* ShouldBeDataDriven::CreateBullet(b2Body* owner, const GameContext& context, bool isPlayer)
{
	// Create a new quad for the purposes of this.
	MoveableTexturedQuad* testQuad = new MoveableTexturedQuad(context.GetTextureManager().GetTexture(isPlayer ? "playerBullet" : "invaderBullet"));
	testQuad->SetSizeMode(MoveableTexturedQuad::SIZE_ACTUAL_SIZE);
	testQuad->SetPositionOffset(Eigen::Vector2f(-0.5f, -0.5f));
	testQuad->SetSizeScale(Eigen::Vector2f(4, 16));
	
	// Create our bullet.
	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = owner->GetPosition();
	b2FixtureDef fixDef;
	b2PolygonShape shape;
	shape.SetAsBox(2 / BOX2D_SCALE_FACTOR, 6 / BOX2D_SCALE_FACTOR);
	fixDef.shape = &shape;
	fixDef.filter = isPlayer ? GamePhysicsConstants::GetPlayerBulletFilter() : GamePhysicsConstants::GetInvaderBulletFilter();
	fixDef.isSensor = true;
	b2Body* testBody = context.GetBox2DWorld().CreateBody(&bodyDef);
	testBody->CreateFixture(&fixDef);

	// Aaaand connect it to some components to make it work.
	Entity* testPhysicsEntity = context.GetComponentManager().GetFreeEntity();
	Box2DBodyComponent* bodyComponent = context.GetComponentManager().AddComponent<Box2DBodyComponent>(testPhysicsEntity);
	bodyComponent->SetBody(testBody);
	Bullet* bullet = context.GetComponentManager().AddComponent<Bullet>(testPhysicsEntity);
	bullet->SetSpeed(isPlayer ? 5.f : -5.f);
	bullet->SetDamage(10);
	bullet->SetOwnerType(isPlayer ? Bullet::BOT_PLAYER : Bullet::BOT_INVADER);
	MoveableQuadComponent* quadComponent = context.GetComponentManager().AddComponent<MoveableQuadComponent>(testPhysicsEntity);
	quadComponent->SetQuad(testQuad);
	quadComponent->SetRenderer(&context.GetQuadRenderer());
	testPhysicsEntity->SetPosition(Eigen::Vector3f(0, 0, 1));

	return testPhysicsEntity;
}

void ShouldBeDataDriven::LevelUpInvader(ComponentModel::Entity* invaderEntity, Invader* invader, const GameContext& context)
{
	// Give the invader a new, improved, configuration
	invader->SetupInvader(Invader::InvaderConfig(80, 40, 10, 4));

	// Get the moveable quad and update the texture.
	MoveableQuadComponent* quadComponent = invaderEntity->GetComponentByTypeFast<MoveableQuadComponent>();
	quadComponent->GetQuad()->SetTexture(context.GetTextureManager().GetTexture("opinvader"));
}