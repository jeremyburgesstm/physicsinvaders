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
#include "Core/StateMachine/ThreadedBaseState.h"

// Boost inheritance
#include <boost/noncopyable.hpp>

class RocketScreen;
class ApplicationContext;
namespace Rocket {
	namespace Core {
		class Context;
	}
}

/**
 * FlowNode type that encapsulates the entry and exit of a Rocket document.
 *
 * Currently also handles loading a rocket document, but this should actually be handled elsewhere.
 */
class RocketFlowScreenBase : public ThreadedBaseState<const ApplicationContext*>, public boost::noncopyable
{
public:
	RocketFlowScreenBase(Rocket::Core::Context& rocketContext,
		const char* pathToDocument);
	virtual ~RocketFlowScreenBase(void);

protected:
	virtual void OnEnter(const ApplicationContext*);
	virtual void OnExit(const ApplicationContext*);

	RocketScreen* m_rocketScreen;
};

