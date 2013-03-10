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

// Boost
#include <boost/assert.hpp>

/**
 * \class SimpleBaseState
 * Base class for simple states.
 *
 * Mostly is just an interface - provides some simplistic common functionality.
 */
template <typename UpdateArgType>
class SimpleBaseState
{
public:
	SimpleBaseState() :
		m_waitingToExit(false),
		m_pendingExit(0)
	{}

	/// Called once per update, while the state is active
	virtual void UpdateState(UpdateArgType /*context*/) {};

	/// Provides the number of exits this state has.
	virtual unsigned char GetExitCount() const = 0;

	/// Invoked by the state machine to enter this state
	void Enter(UpdateArgType context)
	{
		m_waitingToExit = false;
		m_pendingExit = 0;
		OnEnter(context);
	}

	/// Invoked by the state machine to exit this tate
	void Exit(UpdateArgType context)
	{
		OnExit(context);
		m_waitingToExit = false;
		m_pendingExit = 0;
	}

	/// Whether we are currently waiting to exit.
	bool IsWaitingToExit() const { return m_waitingToExit; }

	/// What exit we want to go out of
	unsigned char GetPendingExit() const { return m_pendingExit; }

protected:
	/// Called when the state is entered
	virtual void OnEnter(UpdateArgType /*context*/) {};

	/// Called when the state is exited
	virtual void OnExit(UpdateArgType /*context*/) {};

	/// Sets that we want to exit.
	void SetPendingExit(unsigned char pendingExitPoint)
	{
		BOOST_ASSERT(!m_waitingToExit);
		BOOST_ASSERT(pendingExitPoint < GetExitCount());
		m_waitingToExit = true;
		m_pendingExit = pendingExitPoint;
	}

private:
	/// Whether we currently want to exit
	bool m_waitingToExit;

	/// What exit point we want to go out of
	unsigned char m_pendingExit;
};