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

// Base header
#include "ComponentModel/Component.h"

class TurretYokeComponent : public ComponentModel::Component
{
public:
	TurretYokeComponent(void);
	~TurretYokeComponent(void);
	
	/// Sets the desired direction - -1 for left, 1 for right..
	void SetDesiredDirection(float direction) { m_direction = direction; }

	/// Sets whether to fire this frame.
	void Fire() { m_fire = true; }

	/// Initialises the component
	virtual void Initialise(const GameContext& gameContext);
	
	/// Nullifies all internal refs
	virtual void Cleanup(const GameContext& gameContext);

	/// Does whatever needs to be done to the turret itself.
	virtual void PhysicsUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Sets last time of shot so first shot can be fired when necessary.
	virtual void FirstCoreUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Sets the maximum speed of the turret.
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }

	/// Sets how long the turret needs to wait between shots in ticks (milliseconds)
	void SetFireDelay(AppTicks fireDelay) { m_fireDelay = fireDelay; }

public:
	static bool HasPhysicsUpdate() { return true; }
	static bool HasCoreUpdate() { return false; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return false; }

private:
	Box2DBodyComponent* m_body;
	float m_direction;
	bool m_fire;
	float m_maxSpeed;
	AppTicks m_fireDelay;
	AppTicks m_lastFire;
};

