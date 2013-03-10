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

#include "ApplicationTime.h"

#include <boost/numeric/conversion/cast.hpp>

ApplicationTime* ApplicationTime::s_globalTime = NULL;

ApplicationTime::ApplicationTime(void)
{
	m_startTime = clock();
}

AppTicks ApplicationTime::GetApplicationTime()
{
	return clock() - m_startTime;
}

void ApplicationTime::InitialiseGlobalTime()
{
	s_globalTime = new ApplicationTime();
}

void ApplicationTime::ShutdownGlobalTime()
{
	delete s_globalTime;
}

AppTicks ApplicationTime::GetAbsoluteApplicationTime()
{
	return s_globalTime->GetApplicationTime();
}

float ApplicationTime::ConvertTicksToSeconds(AppTicks time)
{
	// Force cast macro to float to ensure floating point return value.
	return (time / (float)CLOCKS_PER_SEC);
}

AppTicks ApplicationTime::ConvertSecondsToTicks(float time)
{
	return boost::numeric_cast<AppTicks, float>(time * CLOCKS_PER_SEC);
}