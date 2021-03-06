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

#include "RocketFlowScreenClickBody.h"

#include "Rocket/ProgressScreen.h"

RocketFlowScreenClickBody::RocketFlowScreenClickBody(Rocket::Core::Context& rocketContext,
	const char* pathToDocument,
	const char* elementToListenTo,
	const char* eventToListenFor) :
	ThreadedBaseState<const ApplicationContext*>()
{
	m_screen = new ProgressScreen(rocketContext, pathToDocument, elementToListenTo, eventToListenFor);
}

RocketFlowScreenClickBody::~RocketFlowScreenClickBody(void)
{
	delete m_screen;
}

void RocketFlowScreenClickBody::OnEnter(const ApplicationContext*)
{
	m_screen->ClearReadyToExit();
	m_screen->Show();
}

void RocketFlowScreenClickBody::OnExit(const ApplicationContext*)
{
	m_screen->Hide();
}

void RocketFlowScreenClickBody::CoreUpdate(const ApplicationContext*)
{
	if (m_screen->GetReadyToExit())
	{
		SetPendingExit(0);
	}
}