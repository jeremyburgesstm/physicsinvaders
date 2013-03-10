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

// My header
#include "ScaledTime.h"

// Helpers
#include "Utility/ApplicationTime.h"
#include "Utility/Helpers.h"

ScaledTime::ScaledTime(ITimeSource* base) :
	m_base(base),
	m_timeScale(1),
	m_fixedStepFrame(false),
	m_forcedStepTime(0),
	m_maxTimeStep(1000)
{}

AppTicks ScaledTime::GetCurrentTime() const
{
	return m_currentTime;
}

AppTicks ScaledTime::GetFrameDelta() const
{
	return m_lastDelta;
}

float ScaledTime::GetFrameDeltaSeconds() const
{
	return ApplicationTime::ConvertTicksToSeconds(m_lastDelta);
}

void ScaledTime::FrameStarted()
{
	if (!m_fixedStepFrame)
	{
		m_lastDelta = Helpers::Min((AppTicks)(m_base->GetFrameDelta() * m_timeScale), m_maxTimeStep);
	}
	else 
	{
		m_lastDelta = m_forcedStepTime;
		m_fixedStepFrame = false;
	}
	
	m_currentTime += m_lastDelta;
}