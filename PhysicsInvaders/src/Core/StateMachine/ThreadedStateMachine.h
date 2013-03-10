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

// Base header
#include "SimpleStateMachine.h"

// Base state header
#include "ThreadedBaseState.h"

/**
 * \class ThreadedStateMachine
 *
 * Child class for a SimpleStateMachine which implements the pattern found
 * elsewhere in this framework (Core/Synchronise/Render), ensuring that
 * the state machine's functional update is done during Synchronise.
 */
template<typename UpdateArgType>
class ThreadedStateMachine : SimpleStateMachine<UpdateArgType, ThreadedBaseState<UpdateArgType> >
{
public:
	void CoreUpdate(UpdateArgType context)
	{
		m_currentState->CoreUpdate(context);
	}

	void SynchroniseRenderData(UpdateArgType context)
	{
		// Invoke the standard state machine update
		UpdateStates(context);
		m_currentState->SynchroniseRenderData(context);
	}

	void RenderUpdate(UpdateArgType context)
	{		
		m_currentState->RenderUpdate(context);
	}

	void AddState(ThreadedBaseState<UpdateArgType>* state, bool initialState = false)
	{
		SimpleStateMachine::AddState(state, initialState);
	}

	void ConnectStates(ThreadedBaseState<UpdateArgType>* state1, ThreadedBaseState<UpdateArgType>* state2, unsigned char exitn)
	{
		SimpleStateMachine::ConnectStates(state1, state2, exitn);
	}
};