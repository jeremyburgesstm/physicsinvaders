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
class GameMessageHub;
class MoveableQuadComponent;

// Base header
#include "ComponentModel/Component.h"

// Game event types header
#include "Game/Messaging/GameEventTypes.h"

// STL
#include <vector>
#include <map>
#include <list>

/**
 * Class which manages the state of an invader, and handles notifications regarding it.
 */
class Invader : public ComponentModel::Component
{
public:
	struct InvaderConfig
	{
		InvaderConfig(unsigned int score, unsigned int health, unsigned int multiplierChange, unsigned int type) :
			m_baseScore(score),
			m_baseHealth(health),
			m_multiplierChange(multiplierChange),
			m_invaderType(type)
		{ }

		unsigned int m_baseScore;
		unsigned int m_baseHealth;
		unsigned int m_multiplierChange;
		unsigned int m_invaderType;
	};
	
	enum DeathCause
	{
		DC_BULLET,
		DC_FLOOR
	};

public:
	Invader(void);
	
	/// Initialises the component
	virtual void Initialise(const GameContext& gameContext);
	
	/// Nullifies all internal refs
	virtual void Cleanup(const GameContext& gameContext);

	/// Waits until health drops to 0 (as a result of collisions with
	/// bullets or walls...), and then kills the invader.
	virtual void CoreUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Sets up this invader.
	void SetupInvader(const InvaderConfig& config);
	/// Gets the invaders config
	const InvaderConfig& GetConfig() const { return m_config; }

	/// Sets whether this invader is connected to the root
	void SetConnectedToRoot(bool connectedToRoot) { m_isConnectedToRoot = connectedToRoot; }
	/// Gets whether this invader is connected to the root
	bool GetConnectedToRoot() const { return m_isConnectedToRoot; }
	
	/// Sets whether this invader is powered
	void SetPowered(bool powered) { m_isPowered = powered; }
	/// Gets whether this invader is powered
	bool GetPowered() const { return m_isPowered; }

	/// Adds a connected invader
	void AddConnectedEntity(Invader* invader); 
	/// Gets how many connected invaders there are.
	size_t GetConnectedEntityCount() const;
	/// Gets the invader at index i.
	Invader* GetConnectedEntity(size_t i) const;

	/// Gets how we died
	DeathCause GetDeathCause() const { return m_deathCause; }

	/// Tells this invader to fire this frame.
	void Fire() { m_fireThisFrame = true; }
public:
	static bool HasPhysicsUpdate() { return false; }
	static bool HasCoreUpdate() { return true; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return false; }

private:
	void OnCollide(const PhysicsContactEvent& contactEvent);
	void OnConnectedInvaderEvent(GameEventTypes::GameEvent, ComponentModel::Entity*);
	void OnBulletDiedEvent(GameEventTypes::GameEvent, ComponentModel::Entity*);
	void ClearDestroyedInvaders(GameMessageHub& hub);
	void UnregisterBullet(GameMessageHub& hub);
	void LevelUp(const GameContext& context);
	float GetCurrentAlpha();
	
private:
	// Body (for collision events)
	b2Body* m_body;
	
	// Definitions of the invader itself.
	InvaderConfig m_config;
	unsigned int m_health;

	// Whether we're connected to the root
	bool m_isConnectedToRoot;

	// Whether we're powered.
	bool m_isPowered;

	// Whether we should fire this frame.
	bool m_fireThisFrame;

	// Bullet is dead?
	bool m_bulletDead;

	// Should we level up next update?
	bool m_levelUp;

	// What killed us
	DeathCause m_deathCause;

	// Vector of other invaders we're connected to
	std::vector<Invader*> m_connectedInvaders;
	// Mapping of invaders to entities - destruction order could cause badness, so we need to
	// actually know which invader maps to which entity.
	std::map<Invader*, ComponentModel::Entity*> m_invaderMap;
	// List of destroyed invaders so we can safely unsubscribe from them during our update.
	std::list<Invader*> m_destroyedInvaders;

	// Entity which is the bullet we listen to - we can only have one at a time.
	ComponentModel::Entity* m_bullet;
	
	// Visual
	MoveableQuadComponent* m_image;
};

