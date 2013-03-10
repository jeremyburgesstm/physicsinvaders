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

// Required STL includes
#include <vector>
#include <map>

// Boost
#include <boost/assert.hpp>

// Base state header
#include "SimpleBaseState.h"

/**
 * \class SimpleStateMachine
 * 
 * A simple state machine class. Contains a list of states connected via
 * numbered exits. States are fundamentally ignorant of where they're going
 * making it simple to reorganise the state graph.
 *
 * Template argument is the thing you feed to the "UpdateStates" method, and it
 * is in turn fed to all state update methods.
 *
 * ----------    ----------
 * |       1| -> | State2 |
 * |        |    ----------
 * | State1 |
 * |        |    ----------
 * |       2| -> | State3 |
 * ----------    ----------
 *
 * Each state exposes some number of exits, and each exit can be connected to 
 * exactly one other state's entry.
 *
 * In theory we could also give each state multiple entries to provide context, 
 * however for my purposes we don't need that, so don't bother!
 *
 * Once states have been added, the state machine owns them, and will delete them
 * when it is destroyed.
 */
template <typename UpdateArgType, typename StateType = SimpleBaseState<UpdateArgType> >
class SimpleStateMachine
{
public:
	SimpleStateMachine() 
		: m_currentState(nullptr),
		m_started(false)
	{ }

	~SimpleStateMachine()
	{
		for (auto linkIt = m_links.begin(); linkIt != m_links.end(); ++linkIt)
		{
			delete (*linkIt).first;
			delete (*linkIt).second;
		}
		
		// No-one should be accessing this after destruction, but for cleanliness sake, clean out the lists.
		m_currentState = nullptr;
		m_links.clear();
	}

	/// Adds a state to the state machine.
	void AddState(StateType* state, bool isInitialState = false)
	{
		BOOST_ASSERT(!m_started);
		m_links[state] = new std::vector<StateType*>();
		m_links[state]->assign(state->GetExitCount(), nullptr);
		if (isInitialState)
		{
			m_currentState = state;
		}
	}

	/// Connects states 1 & 2 via exit n on state 1
	void ConnectStates(StateType* state1, StateType* state2, unsigned char exitn)
	{
		BOOST_ASSERT(m_links[state1]->size() > exitn);
		BOOST_ASSERT(m_links[state1]->at(exitn) == nullptr);
		(*m_links[state1])[exitn] = state2;
	}

	/// Updates the state machine
	void UpdateStates(UpdateArgType context)
	{		
		if (!m_started)
		{
			Start(context);
		}
		UpdateTransitions(context);
		if (m_currentState != nullptr)
		{
			m_currentState->UpdateState(context);
		}
	}

private:
	/// Updates all transitions
	void UpdateTransitions(UpdateArgType context)
	{
		while (m_currentState != nullptr && m_currentState->IsWaitingToExit())
		{
			StateType* targetState = m_links[m_currentState]->at(m_currentState->GetPendingExit());
			m_currentState->Exit(context);
			m_currentState = targetState;
			if (m_currentState != nullptr)
			{
				m_currentState->Enter(context);
			}
		}
	}

	/// Starts the state machine
	void Start(UpdateArgType context)
	{
		if (m_currentState != nullptr)
		{
			m_currentState->Enter(context);
			m_started = true;
		}
	}
	
protected:	
	/// Currently active state.
	StateType* m_currentState;

private:
	/// Map of all states to their links, also the canonical storage of all states.
	std::map<StateType*, std::vector<StateType*>* > m_links;

	/// Whether we've started or not.
	bool m_started;
};