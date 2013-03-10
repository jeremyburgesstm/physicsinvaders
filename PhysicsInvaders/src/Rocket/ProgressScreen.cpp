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
#include "ProgressScreen.h"

// Rocket
#include <Rocket/Core.h>

ProgressScreen::ProgressScreen(Rocket::Core::Context& rocketContext,
		const char* pathToDocument,
		const char* elementToListenTo,
		const char* eventToProgressOn) :
	RocketScreen(rocketContext, pathToDocument),
	m_readyToExit(false),
	m_eventToProgressOn(eventToProgressOn)
{
	m_element = m_rocketDocument->GetElementById(elementToListenTo);
	m_element->AddEventListener(eventToProgressOn, this);
}

ProgressScreen::~ProgressScreen()
{
	m_element->RemoveEventListener(m_eventToProgressOn.c_str(), this);
}

void ProgressScreen::ProcessEvent(Rocket::Core::Event& /*event*/)
{
	m_readyToExit = true;
}

void ProgressScreen::SetTextOnElement(const char* elementName, const Rocket::Core::WString& string)
{
	Rocket::Core::ElementText* element = (Rocket::Core::ElementText*)m_rocketDocument->GetElementById(elementName)->GetFirstChild();
	element->SetText(string); 
}