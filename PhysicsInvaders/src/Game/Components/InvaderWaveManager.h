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

class Invader;
class InvaderWaveMover;

// Base header
#include "ComponentModel/Component.h"

// Game event types header
#include "Game/Messaging/GameEventTypes.h"

// STL
#include <map>

class InvaderWaveManager : public ComponentModel::Component
{
public:
	struct InvaderWaveConfig
	{
		float m_baseDifficulty;
		float m_difficultyIncrementPerInvaderDeath;
		float m_minFireRate;
		float m_maxFireRate;
	};
public:
	InvaderWaveManager(void);
	
	/// Initialises the component
	virtual void Initialise(const GameContext& gameContext);
	
	/// Nullifies all internal refs
	virtual void Cleanup(const GameContext& gameContext);

	/// Zeros the shoot timer.
	virtual void FirstCoreUpdate(const GameTime& time, const GameContext& gameContext);

	/// Waits until health drops to 0 (as a result of collisions with
	/// bullets or walls...), and then kills the invader.
	virtual void CoreUpdate(const GameTime& time, const GameContext& gameContext);

	/// Adds a connected invader
	void AddInvader(ComponentModel::Entity* invader); 

	/// Sets the wave config
	void SetConfig(const InvaderWaveConfig& config) { m_config = config; }

public:
	static bool HasPhysicsUpdate() { return false; }
	static bool HasCoreUpdate() { return true; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return false; }

private:
	void OnEntityDestroyedEvent(GameEventTypes::GameEvent, ComponentModel::Entity*);
	void UpdatePoweredStatus();
	float GetCurrentFireRate() const;

private:
	// Whether we need to re-calculate the powered status.
	bool m_recalculatePower;
	// Mapping of all entities to relevant invaders (pre-cached for fast lookup)
	std::map<ComponentModel::Entity*, Invader*> m_entityInvaders;
	// Cache of our invader wave mover.
	InvaderWaveMover* m_mover;
	// Count of invaders which have died since we began.
	unsigned int m_numberOfDeadInvaders;
	// Number of current powered invaders
	size_t m_numberOfPoweredInvaders;
	// Cached current difficulty.
	float m_currentDifficulty;
	// Time we last fired
	AppTicks m_timeLastFired;
	// Invader wave config
	InvaderWaveConfig m_config;
};

