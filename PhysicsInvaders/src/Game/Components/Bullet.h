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

class b2Body;
struct PhysicsContactEvent;

// Base header
#include "ComponentModel/Component.h"

// Game event types
#include "Game/Messaging/GameEventTypes.h"

class Bullet : public ComponentModel::Component
{
public:
	enum BulletHitType
	{
		BHT_NONE,
		BHT_OUT_OF_BOUNDS,
		BHT_INVADER,
		BHT_PLAYER
	};

	enum BulletOwnerType
	{
		BOT_INVADER,
		BOT_PLAYER
	};

public:
	Bullet(void);
	
	/// Initialises the component
	virtual void Initialise(const GameContext& gameContext);
	
	/// Nullifies all internal refs
	virtual void Cleanup(const GameContext& gameContext);
		
	/// Moves the bullet, or destroys it when necessary.
	virtual void PhysicsUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// \name Accessors
	/// @{
		
		void SetSpeed(float speed) { m_speed = speed; }
		void SetDamage(unsigned int damage) { m_damage = damage; }
		unsigned int GetDamage() const { return m_damage; }
		BulletOwnerType GetOwnerType() const { return m_ownerType; }
		void SetOwnerType(BulletOwnerType owner) { m_ownerType = owner; }
		BulletHitType GetHitType() const { return m_hitType; }

	/// @}

public:
	static bool HasPhysicsUpdate() { return true; }
	static bool HasCoreUpdate() { return false; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return false; }

private:
	void OnCollide(const PhysicsContactEvent& contactEvent);
	void OnAllInvadersDestroyed(GameEventTypes::GameEvent, ComponentModel::Entity*);
	bool TestBoundary(float currY);

private:
	// Body (for collision events)
	b2Body* m_body;

	// Speed for the bullet
	float m_speed;

	// Damage for the bullet.
	unsigned int m_damage;

	// Whether we've been destroyed
	bool m_dead;

	// Owner type
	BulletOwnerType m_ownerType;

	// Hit type
	BulletHitType m_hitType;
};