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
class Box2DBodyComponent;
struct PhysicsContactEvent;
class b2Body;

// Base header
#include "ComponentModel/Component.h"

// Box2D types
#include <Box2D/Common/b2Math.h>

// STL 
#include <list>

/// Class which moves the invader wave around. Has an internal state machine to manage how it moves.
class InvaderWaveMover : public ComponentModel::Component
{
private:
	struct WallContactEvent
	{
		bool m_wallWasLeft;
	};

public:
	class MovementPhase
	{
	public:
		virtual bool ShouldContinue(const std::list<WallContactEvent>& events, AppTicks timeInPhase, InvaderWaveMover* mover, Box2DBodyComponent* body) = 0;
		virtual void BecomeActive(InvaderWaveMover* mover, Box2DBodyComponent* body) = 0;
		virtual void DoWork(InvaderWaveMover* mover, Box2DBodyComponent* body, const GameTime& time) = 0;
	};
	
	class MovementPhasePause : public MovementPhase
	{
	public:
		MovementPhasePause(AppTicks timeToPause) : m_timeToPause(timeToPause) {}

		virtual bool ShouldContinue(const std::list<WallContactEvent>& events, AppTicks timeInPhase, InvaderWaveMover* mover, Box2DBodyComponent* body);
		virtual void BecomeActive(InvaderWaveMover* /*mover*/, Box2DBodyComponent* /*body*/);
		virtual void DoWork(InvaderWaveMover* /*mover*/, Box2DBodyComponent* /*body*/, const GameTime& /*time*/) {}
	private:
		AppTicks m_timeToPause;
	};

	class MovementPhaseSidewaysToWall : public MovementPhase
	{
	public:
		MovementPhaseSidewaysToWall(bool goLeft) : m_leftWallIsTarget(goLeft) {}

		virtual bool ShouldContinue(const std::list<WallContactEvent>& events, AppTicks timeInPhase, InvaderWaveMover* mover, Box2DBodyComponent* body);
		virtual void BecomeActive(InvaderWaveMover* /*mover*/, Box2DBodyComponent* /*body*/);
		virtual void DoWork(InvaderWaveMover* mover, Box2DBodyComponent* body, const GameTime& time);
	private:
		bool m_leftWallIsTarget;
	};

	class MovementPhaseDescendByDistance : public MovementPhase
	{
	public:
		MovementPhaseDescendByDistance(float distance) : m_distance(distance), m_yOnEnter(0) {}

		virtual bool ShouldContinue(const std::list<WallContactEvent>& events, AppTicks timeInPhase, InvaderWaveMover* mover, Box2DBodyComponent* body);
		virtual void BecomeActive(InvaderWaveMover* mover, Box2DBodyComponent* body);
		virtual void DoWork(InvaderWaveMover* mover, Box2DBodyComponent* body, const GameTime& time);
	private:
		float m_distance;
		float m_yOnEnter;
	};

	struct InvaderMoverConfig
	{
		InvaderMoverConfig() :
			m_minMoverAcceleration(0), m_maxMoverAcceleration(0),
			m_minMoverMaxSpeed(0), m_maxMoverMaxSpeed(0)
		{}

		float m_minMoverAcceleration;
		float m_maxMoverAcceleration;
		float m_minMoverMaxSpeed;
		float m_maxMoverMaxSpeed;
	};

public:
	InvaderWaveMover();
	~InvaderWaveMover();

	/// Initialises the component
	virtual void Initialise(const GameContext& gameContext);
	
	/// Nullifies all internal refs
	virtual void Cleanup(const GameContext& gameContext);

	/// Does whatever needs to be done to the invader wave.
	virtual void PhysicsUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Sets timing correctly 
	virtual void FirstCoreUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Sets the walls
	void SetWalls(Box2DBodyComponent* leftWall, Box2DBodyComponent* rightWall); 

	/// Adds a movement phase
	void AddMovementPhase(MovementPhase* phase) { m_movementPhases.push_back(phase); }

	/// Sets the current config.
	void SetConfig(const InvaderMoverConfig& config) { m_config = config; }

	/// Sets the current difficulty
	void SetDifficulty(float difficulty) { m_currentDifficulty = difficulty; }

public:
	static bool HasPhysicsUpdate() { return true; }
	static bool HasCoreUpdate() { return false; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return false; }

private:
	/// Callback for contact started events - we listen for all contacts involving invaders.
	void OnHitLeftWall(const PhysicsContactEvent& contactEvent);
	void OnHitRightWall(const PhysicsContactEvent& contactEvent);
	float GetCurrentMaxSpeed() const;
	float GetCurrentMaxSpeedSquared() const;
	float GetCurrentAcceleration() const;

private:
	// All physics bodies this needs to keep track of.
	Box2DBodyComponent* m_body;
	b2Body* m_leftWall;
	b2Body* m_rightWall;
	
	// Invader wave config
	InvaderMoverConfig m_config;

	// State management
	AppTicks m_timeInCurrentPhase;
	b2Vec2 m_velocity;
	float m_currentDifficulty;

	// Movement phases.
	std::list<MovementPhase*> m_movementPhases;
	std::list<MovementPhase*>::iterator m_currentMovementPhase;

	// Wall contact events
	std::list<WallContactEvent> m_wallContactEvents;
};