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
#include "TurretController.h"

// Other game headers.
#include "CoreComponents/Box2DBodyComponent.h"
#include "CoreComponents/MoveableQuadComponent.h"
#include "Core/Functional/Action.h"
#include "Core/GameTime.h"
#include "Core/ScaledTime.h"
#include "Utility/ApplicationTime.h"
#include "Game/GameContext.h"
#include "Game/Messaging/PhysicsContactEvent.h"
#include "Game/Messaging/GameMessageHub.h"
#include "Game/Physics/GamePhysicsConstants.h"
#include "Invader.h"

// Box 2D
#include <Box2D/Box2D.h>

TurretController::TurretController() :
	m_body(nullptr),
	m_killingEntity(nullptr),
	m_image(nullptr),
	m_numLives(0),
	m_hitThisFrame(false)
{}

TurretController::~TurretController()
{}

void TurretController::Initialise(const GameContext& gameContext)
{
	m_body = m_entity->GetComponentByTypeFast<Box2DBodyComponent>()->GetBody();
	m_image = m_entity->GetComponentByTypeFast<MoveableQuadComponent>();
	m_hitThisFrame = false;
	m_killingEntity = nullptr;
	m_invulnerable = false;
	GameMessageHub::PhysicsInterestRegistration interest;
	interest.m_bodyOfInterest = m_body;
	gameContext.GetMessageHub().SubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &TurretController::OnCollide));
}

void TurretController::Cleanup(const GameContext& gameContext)
{
	m_body = nullptr;
	m_killingEntity = nullptr;
	m_image = nullptr;
	GameMessageHub::PhysicsInterestRegistration interest;
	interest.m_bodyOfInterest = m_body;
	gameContext.GetMessageHub().UnsubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &TurretController::OnCollide));	
}

void TurretController::CoreUpdate(const GameTime& time, const GameContext& gameContext)
{
	if (m_hitThisFrame)
	{
		Die(time, gameContext, m_killingEntity->GetComponentByTypeFast<Invader>() != nullptr);
	}
	if (m_invulnerable && m_timeToBecomeVulnerable < time.GetGameTime()->GetCurrentTime())
	{
		BecomeVulnerable();
	}
}

void TurretController::OnCollide(const PhysicsContactEvent& contactEvent)
{
	unsigned short category = contactEvent.m_fixtureB->GetFilterData().categoryBits;
	if (!m_hitThisFrame)		
	{		
		// Get the entity that hit us
		ComponentModel::Entity* entity = (ComponentModel::Entity*)(contactEvent.m_fixtureB->GetBody()->GetUserData());
		// If it's an invader, only look at it if it's powered - if it's a bullet, it's always a successful hit.
		if (category == GamePhysicsConstants::c_invaderUnitLayer)
		{
			Invader* invader = entity->GetComponentByTypeFast<Invader>();
			m_hitThisFrame = invader->GetPowered();
		}
		else
		{
			m_hitThisFrame = category == GamePhysicsConstants::c_invaderBulletLayer;
		}

		// If we were hit this frame, store the killer.
		if (m_hitThisFrame)
		{
			m_killingEntity = entity;
		}
	}
}

void TurretController::SetTimeToRemainInvulnerableAfterDeath(float time)
{
	m_timeToRemainInvulnerableAfterDying = ApplicationTime::ConvertSecondsToTicks(time);
}

void TurretController::SetInvulnerable(bool vulnerable)
{
	m_invulnerable = vulnerable;
	SetFilterInvulnerable(vulnerable);
	if (vulnerable)
	{
		m_image->SetColour(Eigen::Vector4f(1, 1, 1, 0.5f));
		m_killingEntity = nullptr;
	}
	else
	{
		m_image->SetColour(Eigen::Vector4f(1, 1, 1, 1));
	}
}

void TurretController::Die(const GameTime& time, const GameContext& gameContext, bool instaFail)
{
	m_hitThisFrame = false;
	if (!instaFail && --m_numLives > 0)
	{
		m_timeToBecomeVulnerable = time.GetGameTime()->GetCurrentTime() + m_timeToRemainInvulnerableAfterDying;
		SetInvulnerable(true);
		gameContext.GetMessageHub().RaiseGameEvent(GameEventTypes::GE_PLAYER_LOST_LIFE, m_entity);
	}
	else
	{
		m_numLives = 0;
		gameContext.GetMessageHub().RaiseGameEvent(GameEventTypes::GE_PLAYER_DEFEATED, m_entity);
		// Hang about - don't actually die.
		m_entity->SetEnabled(false);
	}
}

void TurretController::BecomeVulnerable()
{
	SetInvulnerable(false);
}

void TurretController::SetFilterInvulnerable(bool invulnerable)
{
	b2Fixture* fixture = m_body->GetFixtureList();
	while (fixture != nullptr)
	{
		fixture->SetFilterData(invulnerable ? GamePhysicsConstants::GetPlayerUnitInvulnFilter() : GamePhysicsConstants::GetPlayerUnitFilter());
		fixture = fixture->GetNext();
	}
}