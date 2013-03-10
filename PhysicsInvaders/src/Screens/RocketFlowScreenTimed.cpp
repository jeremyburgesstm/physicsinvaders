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

#include "RocketFlowScreenTimed.h"

#include <time.h>
#include <boost/assert.hpp>
#include "Utility/ApplicationTime.h"

RocketFlowScreenTimed::RocketFlowScreenTimed(Rocket::Core::Context& rocketContext,
	const char* pathToDocument,
	float time) :
	RocketFlowScreenBase(rocketContext, pathToDocument),
	m_time(time),
	m_entryTime(0)
{}

RocketFlowScreenTimed::~RocketFlowScreenTimed(void)
{}

void RocketFlowScreenTimed::OnEnter(const ApplicationContext* context)
{
	RocketFlowScreenBase::OnEnter(context);
	m_entryTime = ApplicationTime::GetAbsoluteApplicationTime();
}

void RocketFlowScreenTimed::CoreUpdate(const ApplicationContext*)
{
	// Now compare amount of time against time we're supposed to wait.
	float timeInNode = ApplicationTime::ConvertTicksToSeconds(ApplicationTime::GetAbsoluteApplicationTime() - m_entryTime);
	if (timeInNode >= m_time)
	{
		SetPendingExit(0);
	}
}