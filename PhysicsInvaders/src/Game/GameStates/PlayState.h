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
class GameStateContext;
namespace ComponentModel
{
	class Entity;
};

// Base class
#include "Core/StateMachine/ThreadedBaseState.h"

// Game event types
#include "Game/Messaging/GameEventTypes.h"

/**
 * \class PlayState
 *
 * State which just lets the game play, and waits for stage ending events.
 */
class PlayState : public ThreadedBaseState<const GameStateContext*>
{
public:
	enum ExitPoints
	{
		STATE_EXIT_PLAYER_WON = 0,
		STATE_EXIT_PLAYER_LOST = 1
	};

public:
	PlayState() : m_lastScore(0) {}

	/// Provide exit count, as required
	unsigned char GetExitCount() const { return 2; }

	/// Override update to update the HUD
	void UpdateState(const GameStateContext*);

protected:
	/// Override OnEnter to do work (pause time).
	void OnEnter(const GameStateContext*);

	/// Override OnExit to do work (unpause time).
	void OnExit(const GameStateContext*);

private:
	/// Event handler for Player won wave
	void OnPlayerWonEvent(GameEventTypes::GameEvent, ComponentModel::Entity*);

	/// Event handler for Player defeated by wave
	void OnPlayerLostEvent(GameEventTypes::GameEvent, ComponentModel::Entity*);

private: 

	float m_lastScore;
	static const int c_minMoveRate = 10;
	static const int c_maxMoveRate = 500;
	static const int c_minScoreForEval = 10;
	static const int c_maxScoreForEval = 1000;
};