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

class GameContext;
class GameTime;
class StateOfTheGame;
class HUDScreen;
class ProgressScreen;

// Not a copyable class
#include <boost/noncopyable.hpp>

/**
 * \class GameStateContext
 * This class acts as a 'hub' class for states. It basically has the 
 * normal game context, and some additional stuff that states in the game 
 * state machine require.
 */
class GameStateContext : public boost::noncopyable
{
public: 
	GameStateContext(GameContext& gameContext, 
		GameTime& time,
		StateOfTheGame& state,
		HUDScreen& hud,
		ProgressScreen& victory,
		ProgressScreen& defeat) :
		m_gameContext(gameContext),
		m_gameTime(time),
		m_stateOfTheGame(state),
		m_hud(hud),
		m_victory(victory),
		m_defeat(defeat)
	{}

	/// Returns a reference to the standard game context.
	GameContext& GetGameContext() const { return m_gameContext; }

	/// Returns a (non-const) reference to the game time.
	GameTime& GetGameTime() const { return m_gameTime; }

	/// Gets the current state of the game.
	StateOfTheGame& GetStateOfTheGame() const { return m_stateOfTheGame; }

	/// Gets the hud
	HUDScreen& GetHud() const { return m_hud; }

	/// Gets the hud
	ProgressScreen& GetVictory() const { return m_victory; }

	/// Gets the hud
	ProgressScreen& GetDefeat() const { return m_defeat; }

private:
	GameContext& m_gameContext;
	GameTime& m_gameTime;
	StateOfTheGame& m_stateOfTheGame;
	HUDScreen& m_hud;
	ProgressScreen& m_victory;
	ProgressScreen& m_defeat;
};