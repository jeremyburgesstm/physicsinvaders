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
class b2Body;
struct PhysicsContactEvent;
class MoveableQuadComponent;

// Base header
#include "ComponentModel/Component.h"

/**
 * Class which manages the state of a user turret. Tracks how many lives the user has,
 * and generates appropriate events when the user is hit.
 */
class TurretController : public ComponentModel::Component
{
public:
	TurretController(void);
	~TurretController(void);
	
	/// Initialises the component
	virtual void Initialise(const GameContext& gameContext);
	
	/// Nullifies all internal refs
	virtual void Cleanup(const GameContext& gameContext);

	/// Does whatever needs to be done to the turret itself.
	virtual void CoreUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Sets how many lives this user has
	void SetNumLives(int numLives) { m_numLives = numLives; }

	/// Sets how long we remain invulnerable for
	void SetTimeToRemainInvulnerableAfterDeath(float time);

	/// Gets the entity that killed us
	ComponentModel::Entity* GetKillingEntity() const { return m_killingEntity; }

public:
	static bool HasPhysicsUpdate() { return false; }
	static bool HasCoreUpdate() { return true; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return false; }

private:
	/// Sets whether we're currently invulnerable (i.e., we don't die)
	void SetInvulnerable(bool invulnerable);

	void OnCollide(const PhysicsContactEvent& contactEvent);
	void Die(const GameTime& time, const GameContext& game, bool instaFail);
	void BecomeVulnerable();
	void SetFilterInvulnerable(bool invulnerable);

private:
	b2Body* m_body;
	ComponentModel::Entity* m_killingEntity;
	MoveableQuadComponent* m_image;
	int m_numLives;
	bool m_hitThisFrame;
	bool m_waveHitThisFrame;
	bool m_invulnerable;
	AppTicks m_timeToBecomeVulnerable;
	AppTicks m_timeToRemainInvulnerableAfterDying;
};

