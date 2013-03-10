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
#include "Bullet.h"

// Project headers
#include "CoreComponents/Box2DBodyComponent.h"
#include "Core/GameTime.h"
#include "Core/ITimeSource.h"
#include "Core/Functional/Action.h"
#include "Game/GameContext.h"
#include "Game/Messaging/GameMessageHub.h"
#include "Game/Messaging/PhysicsContactEvent.h"
#include "Game/Physics/GamePhysicsConstants.h"

// Box 2D
#include <Box2D/Box2D.h>

Bullet::Bullet() :
	m_body(nullptr),
	m_speed(0)
{}

void Bullet::Initialise(const GameContext& gameContext)
{
	m_dead = false;
	m_hitType = BHT_NONE;
	m_body = m_entity->GetComponentByType<Box2DBodyComponent>()->GetBody();
	GameMessageHub::PhysicsInterestRegistration interest;
	interest.m_bodyOfInterest = m_body;
	gameContext.GetMessageHub().SubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &Bullet::OnCollide));
	gameContext.GetMessageHub().SubscribeGameEvent(GameEventTypes::GE_ALL_INVADERS_DESTROYED, Functional::Creator::CreateAction(this, &Bullet::OnAllInvadersDestroyed));
	
}

void Bullet::Cleanup(const GameContext& gameContext)
{
	GameMessageHub::PhysicsInterestRegistration interest;
	interest.m_bodyOfInterest = m_body;
	gameContext.GetMessageHub().UnsubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &Bullet::OnCollide));
	gameContext.GetMessageHub().UnsubscribeGameEvent(GameEventTypes::GE_ALL_INVADERS_DESTROYED, Functional::Creator::CreateAction(this, &Bullet::OnAllInvadersDestroyed));
	m_body = nullptr;
}

void Bullet::PhysicsUpdate(const GameTime& time, const GameContext& /*gameContext*/)
{
	b2Vec2 pos = m_body->GetPosition();
	if (!m_dead && TestBoundary(pos.y))
	{
		m_hitType = BHT_OUT_OF_BOUNDS;
		m_dead = true;
	}

	if (m_dead)
	{
		m_entity->Destroy();
		// Also disable this - Physics update can keep being called otherwise.
		SetEnabled(false);
		return;
	}

	pos.y += time.GetStepInSeconds() * m_speed;
	m_body->SetTransform(pos, 0);
}

void Bullet::OnCollide(const PhysicsContactEvent& contactEvent)
{
	m_dead = true;
	const b2Filter& filter = contactEvent.m_fixtureB->GetFilterData();
	if (filter.categoryBits == GamePhysicsConstants::c_invaderUnitLayer)
	{
		m_hitType = BHT_INVADER;
	}
	else if (filter.categoryBits == GamePhysicsConstants::c_playerTurretLayer)
	{
		m_hitType = BHT_PLAYER;
	}
	else 
	{
		m_hitType = BHT_OUT_OF_BOUNDS;
	}
}

void Bullet::OnAllInvadersDestroyed(GameEventTypes::GameEvent, ComponentModel::Entity*)
{
	// If we've already been disabled, no need to die again!
	if (m_enabled)
	{
		m_entity->Destroy();
	}
}

bool Bullet::TestBoundary(float currY)
{
	return (m_speed > 0 && currY > (600 / BOX2D_SCALE_FACTOR))
		|| (m_speed < 0 && currY < 0);
}