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

#include "GameTime.h"
#include "Utility/ApplicationTime.h"
#include "Core/RealTime.h"
#include "Core/SteppedTime.h"
#include "Core/ScaledTime.h"

GameTime::GameTime()
{
	m_realTime = new RealTime();
	m_gameTime = new ScaledTime(m_realTime);
	m_physicsTime = new SteppedTime(m_gameTime, ApplicationTime::ConvertSecondsToTicks(1/60.f));
}

GameTime::~GameTime()
{
	delete m_physicsTime;
	delete m_gameTime;
	delete m_realTime;
}

const ITimeSource* GameTime::GetGameTime() const 
{
	return m_gameTime;
}

const ITimeSource* GameTime::GetPhysicsTime() const
{
	return m_physicsTime;
}

long GameTime::GetNumStepsThisFrame() const 
{ 
	return m_physicsTime->GetNumStepsThisFrame(); 
}

float GameTime::GetStepInSeconds() const 
{ 
	return m_physicsTime->GetStepInSeconds(); 
}

AppTicks GameTime::GetStep() const 
{ 
	return m_physicsTime->GetStep(); 
}

void GameTime::FrameStarted()
{
	m_realTime->FrameStarted();
	m_gameTime->FrameStarted();
	m_physicsTime->FrameStarted();
}

void GameTime::SetTimeScale(float timeScale)
{
	m_gameTime->SetTimeScale(timeScale);
}

float GameTime::GetTimeScale() const
{
	return m_gameTime->GetTimeScale();
}

void GameTime::StepNextFrame()
{
	m_gameTime->SetNextForcedUpdateTimeStep(m_physicsTime->GetStep());
}