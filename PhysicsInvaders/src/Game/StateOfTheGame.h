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
class GameMessageHub;
class Invader;
class Bullet;
namespace ComponentModel
{
	class Entity;
};

// Include game event types so that enum type is defined
#include "Messaging/GameEventTypes.h"

// Boost inheritance
#include <boost/noncopyable.hpp>

/**
 * Class which stores the state of the current game for easy access by
 * client systems. 
 * Stores:
 * How many levels there are.
 * What level we're currently on.
 *
 * Stores and updates:
 * Score.
 * Combo.
 */
class StateOfTheGame : public boost::noncopyable
{
public: 
	/// C-Tor - initialises level count.
	StateOfTheGame(unsigned int totalLevels, unsigned int initialPlayerLives, GameMessageHub& messageHub);
	~StateOfTheGame();

	/// \name Accessors
	/// @{
		unsigned int GetScore() const { return m_score; }
		unsigned int GetCombo() const { return m_combo; }
		unsigned int GetCurrentLevel() const { return m_currentLevel; }
		unsigned int GetTotalLevels() const  { return m_totalLevels; }
		unsigned int GetBestScoreThisSession() const { return m_bestScoreThisSession; }
		unsigned int GetNumLives() const { return m_lives; }
		unsigned int GetTotalLives() const { return m_totalLives; }
	/// @}

	/// Increments the current level, and zeroes the combo.
	void IncrementCurrentLevel();

	/// Resets the state (zeroes score, zeroes combo, zeroes level).
	void ResetStateOfTheGame();

private:
	void OnEntityDestroyed(GameEventTypes::GameEvent event, ComponentModel::Entity* entity);
	void OnPlayerLostLife(GameEventTypes::GameEvent event, ComponentModel::Entity* entity);
	void ProcessInvaderDeath(Invader* invader);
	void ProcessBulletDeath(Bullet* bullet);

private:
	unsigned int m_totalLevels;
	unsigned int m_currentLevel;
	unsigned int m_combo;
	unsigned int m_score;
	unsigned int m_bestScoreThisSession;
	unsigned int m_lives;
	unsigned int m_totalLives;
	GameMessageHub& m_messageHub;
};