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

// Base headers
#include "Rocket/RocketScreen.h"
#include <Rocket/Core/EventListener.h>

//STL
#include <string>

namespace Rocket {
	namespace Core {
		class Element;
		class WString;
	}
}

class ProgressScreen : public RocketScreen, Rocket::Core::EventListener
{
public:
	ProgressScreen(Rocket::Core::Context& rocketContext,
		const char* pathToDocument,
		const char* elementToListenTo,
		const char* eventToProgressOn);
	virtual ~ProgressScreen();

	bool GetReadyToExit() const { return m_readyToExit; }
	void ClearReadyToExit() { m_readyToExit = false; }

	void ProcessEvent(Rocket::Core::Event& event);

	void SetTextOnElement(const char* elementName, const Rocket::Core::WString& string);

private:
	Rocket::Core::Element* m_element;
	std::string m_eventToProgressOn;
	bool m_readyToExit;
};