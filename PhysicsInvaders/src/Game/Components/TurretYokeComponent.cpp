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
#include "TurretYokeComponent.h"

// Project headers
#include "CoreComponents/Box2DBodyComponent.h"
#include "Game/GameContext.h"
#include "ShouldBeDataDriven/EntityCreation.h"
#include "Utility/Helpers.h"
#include "Core/GameTime.h"
#include "Core/ITimeSource.h"

// Box 2D
#include <Box2D/Box2D.h>

// Force to apply to move the body - potentially should be a configurable value?.
const float FORCE_TO_APPLY = 50;

TurretYokeComponent::TurretYokeComponent(void) :
	m_direction(0),
	m_body(nullptr),
	m_maxSpeed(1),
	m_fireDelay(1)
{
}

TurretYokeComponent::~TurretYokeComponent(void)
{
}

void TurretYokeComponent::Initialise(const GameContext& /*gameContext*/)
{
	m_body = m_entity->GetComponentByType<Box2DBodyComponent>();
	m_fire = false;
}

void TurretYokeComponent::Cleanup(const GameContext& /*gameContext*/)
{
	m_body = nullptr;
	m_maxSpeed = 0;
}

void TurretYokeComponent::FirstCoreUpdate(const GameTime& time, const GameContext& /*gameContext*/)
{
	m_lastFire = time.GetPhysicsTime()->GetCurrentTime() - m_fireDelay;
}

void TurretYokeComponent::PhysicsUpdate(const GameTime& time, const GameContext& gameContext)
{
	b2Vec2 speed = m_body->GetBody()->GetLinearVelocity();
	if (Helpers::Sign(speed.x) != Helpers::Sign(-m_direction) || abs(speed.x) < m_maxSpeed)
	{
		m_body->ApplyForceToCenter(b2Vec2(m_direction * -FORCE_TO_APPLY, 0));
	}
	if (m_fire && time.GetPhysicsTime()->GetCurrentTime() - m_lastFire > m_fireDelay)
	{
		ShouldBeDataDriven::CreateBullet(m_body->GetBody(), gameContext, true);
		m_lastFire = time.GetPhysicsTime()->GetCurrentTime();
	}
	m_fire = false;
}
