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
class ProgressScreen;

// Base class
#include "Core/StateMachine/ThreadedBaseState.h"

/**
 * \class EndState
 *
 * Shows the victory screen & pauses time.
 */
class EndState : public ThreadedBaseState<const GameStateContext*>
{
public:
	EndState(bool victory) : m_victory(victory) {}

	/// Provide exit count, as required
	unsigned char GetExitCount() const { return 1; }
	
	/// Override update to wait for player to click
	void UpdateState(const GameStateContext*);
protected:
	/// Override OnEnter to show victory screen
	void OnEnter(const GameStateContext*);

	/// Override OnExit to hide victory screen
	void OnExit(const GameStateContext*);
private: 
	ProgressScreen& GetScreen(const GameStateContext*) const;

private:
	bool m_victory;
};