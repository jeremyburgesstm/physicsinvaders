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
#include "InvaderWaveMover.h"

// Project headers
#include "CoreComponents/Box2DBodyComponent.h"
#include "Core/GameTime.h"
#include "Core/ITimeSource.h"
#include "Game/GameContext.h"
#include "Game/Messaging/PhysicsContactEvent.h"
#include "Game/Messaging/GameMessageHub.h"
#include "Game/Physics/GamePhysicsConstants.h"
#include "Core/Functional/Action.h"
#include "Utility/Helpers.h"

// Box 2D
#include <Box2D/Box2D.h>

InvaderWaveMover::InvaderWaveMover(void) :
	m_body(nullptr),
	m_leftWall(nullptr),
	m_rightWall(nullptr),
	m_timeInCurrentPhase(0),
	m_velocity(b2Vec2_zero)
{
}

InvaderWaveMover::~InvaderWaveMover(void)
{
	m_body = nullptr;
	m_leftWall = nullptr;
	m_rightWall = nullptr;
}

void InvaderWaveMover::Initialise(const GameContext& gameContext)
{
	m_velocity = b2Vec2_zero;
	GameMessageHub::PhysicsInterestRegistration interest;
	interest.m_bodyOfInterest = m_leftWall;
	gameContext.GetMessageHub().SubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &InvaderWaveMover::OnHitLeftWall));
	interest.m_bodyOfInterest = m_rightWall;
	gameContext.GetMessageHub().SubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &InvaderWaveMover::OnHitRightWall));
	m_body = m_entity->GetComponentByType<Box2DBodyComponent>();
	m_currentDifficulty = 0;
}

void InvaderWaveMover::Cleanup(const GameContext& gameContext)
{
	GameMessageHub::PhysicsInterestRegistration interest;
	interest.m_bodyOfInterest = m_leftWall;
	gameContext.GetMessageHub().UnsubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &InvaderWaveMover::OnHitLeftWall));
	interest.m_bodyOfInterest = m_rightWall;
	gameContext.GetMessageHub().UnsubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &InvaderWaveMover::OnHitRightWall));
	m_body = nullptr;
	m_leftWall = nullptr;
	m_rightWall = nullptr;

	// Clear out any movement phases.
	for (auto movIt = m_movementPhases.begin(); movIt != m_movementPhases.end(); ++movIt)
	{
		delete *movIt;
	}
	m_movementPhases.clear();
	m_config = InvaderMoverConfig();
}

void InvaderWaveMover::FirstCoreUpdate(const GameTime& /*time*/, const GameContext& /*gameContext*/)
{
	m_timeInCurrentPhase = 0;

	// Initialise the iterator to point to the beginning of the movement phases.
	// Note - something external should have configured the movement phases list by now.
	m_currentMovementPhase = m_movementPhases.begin();
}

void InvaderWaveMover::PhysicsUpdate(const GameTime& time, const GameContext& /*gameContext*/)
{
	// Get the current movement phase.
	MovementPhase* currentPhase = *m_currentMovementPhase;

	// Ask the current phase if it should continue.
	if (!currentPhase->ShouldContinue(m_wallContactEvents, m_timeInCurrentPhase, this, m_body))
	{
		// If not, increment the phase iterator, tell it it's becoming active, and continue.
		if (++m_currentMovementPhase == m_movementPhases.end())
		{
			m_currentMovementPhase = m_movementPhases.begin();
		}
		m_timeInCurrentPhase = 0;
		currentPhase = *m_currentMovementPhase;
		currentPhase->BecomeActive(this, m_body);
	}

	// Tell the phase to do whatever it needs to do.
	currentPhase->DoWork(this, m_body, time);

	// Increment the time in the phase.
	m_timeInCurrentPhase += time.GetStep();

	// Clear the event list.
	m_wallContactEvents.clear();

	// Update the position of the body
	if (!(m_velocity == b2Vec2_zero))
	{
		b2Vec2 pos = m_body->GetBody()->GetPosition() + m_velocity;
		m_body->GetBody()->SetTransform(pos, 0);
	}
}

void InvaderWaveMover::OnHitLeftWall(const PhysicsContactEvent& contactEvent)
{
	if (contactEvent.m_fixtureB->GetFilterData().categoryBits == GamePhysicsConstants::c_invaderUnitLayer)
	{
		WallContactEvent ev;
		ev.m_wallWasLeft = true;
		m_wallContactEvents.push_back(ev);
	}
}

void InvaderWaveMover::OnHitRightWall(const PhysicsContactEvent& contactEvent)
{
	if (contactEvent.m_fixtureB->GetFilterData().categoryBits == GamePhysicsConstants::c_invaderUnitLayer)
	{
		WallContactEvent ev;
		ev.m_wallWasLeft = false;
		m_wallContactEvents.push_back(ev);
	}
}

void InvaderWaveMover::SetWalls(Box2DBodyComponent* leftWall, Box2DBodyComponent* rightWall) 
{ 
	m_leftWall = leftWall->GetBody(); 
	m_rightWall = rightWall->GetBody(); 
}

float InvaderWaveMover::GetCurrentMaxSpeed() const 
{
	return Helpers::Lerp(m_config.m_minMoverMaxSpeed, m_config.m_maxMoverMaxSpeed, m_currentDifficulty);
}

float InvaderWaveMover::GetCurrentMaxSpeedSquared() const 
{
	float maxSpeed = GetCurrentMaxSpeed();
	return maxSpeed * maxSpeed;
}

float InvaderWaveMover::GetCurrentAcceleration() const 
{
	return Helpers::Lerp(m_config.m_minMoverAcceleration, m_config.m_maxMoverAcceleration, m_currentDifficulty);
}

bool InvaderWaveMover::MovementPhasePause::ShouldContinue(const std::list<WallContactEvent>& /*events*/, 
														  AppTicks timeInPhase, 
														  InvaderWaveMover* /*mover*/, 
														  Box2DBodyComponent* /*body*/)
{
	return (timeInPhase < m_timeToPause);
}

void InvaderWaveMover::MovementPhasePause::BecomeActive(InvaderWaveMover* mover, Box2DBodyComponent* /*body*/)
{
	mover->m_velocity = b2Vec2_zero;
}

bool InvaderWaveMover::MovementPhaseSidewaysToWall::ShouldContinue(const std::list<WallContactEvent>& events, 
																   AppTicks /*timeInPhase*/, 
																   InvaderWaveMover* /*mover*/, 
																   Box2DBodyComponent* /*body*/)
{
	// Return true if there are no events in which the wall hit was the wall we're aiming for.
	return std::find_if(events.begin(), 
		events.end(), 
		[this](WallContactEvent wce){ return wce.m_wallWasLeft == m_leftWallIsTarget; }) == events.end(); 
}

void InvaderWaveMover::MovementPhaseSidewaysToWall::BecomeActive(InvaderWaveMover* mover, Box2DBodyComponent* /*body*/)
{
	mover->m_velocity = b2Vec2_zero;
}

void InvaderWaveMover::MovementPhaseSidewaysToWall::DoWork(InvaderWaveMover* mover, 
														   Box2DBodyComponent* /*body*/, 
														   const GameTime& time)
{
	b2Vec2 force;
	force.SetZero();

	int forceMultiplier = m_leftWallIsTarget ? 1 : -1;
	force = b2Vec2(forceMultiplier * mover->GetCurrentAcceleration(), 0);

	mover->m_velocity += time.GetStepInSeconds() * force;
	if (mover->m_velocity.LengthSquared() > mover->GetCurrentMaxSpeedSquared())
	{
		mover->m_velocity.Normalize();
		mover->m_velocity *= mover->GetCurrentMaxSpeed();
	}
}

bool InvaderWaveMover::MovementPhaseDescendByDistance::ShouldContinue(const std::list<WallContactEvent>& /*events*/, 
																	  AppTicks /*timeInPhase*/, 
																	  InvaderWaveMover* /*mover*/, 
																	  Box2DBodyComponent* body)
{
	return body->GetBody()->GetPosition().y > (m_yOnEnter - m_distance);
}

void InvaderWaveMover::MovementPhaseDescendByDistance::BecomeActive(InvaderWaveMover* mover, 
																	Box2DBodyComponent* body)
{
	mover->m_velocity = b2Vec2_zero;
	m_yOnEnter = body->GetBody()->GetPosition().y;
}

void InvaderWaveMover::MovementPhaseDescendByDistance::DoWork(InvaderWaveMover* mover, 
															  Box2DBodyComponent* /*body*/, 
															  const GameTime& time)
{
	b2Vec2 force;
	force.SetZero();

	force = b2Vec2(0, -mover->GetCurrentAcceleration());

	mover->m_velocity += time.GetStepInSeconds() * force;	
	if (mover->m_velocity.LengthSquared() > mover->GetCurrentMaxSpeedSquared())
	{
		mover->m_velocity.Normalize();
		mover->m_velocity *= mover->GetCurrentMaxSpeed();
	}
}
