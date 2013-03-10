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

#include <string>

class PointerEvent
{
public:
	enum POINTER_EVENT_TYPE
	{
		MOVE,
		L_BUTTON_DOWN,
		L_BUTTON_UP,
		R_BUTTON_DOWN,
		R_BUTTON_UP,
		M_BUTTON_DOWN,
		M_BUTTON_UP,
		WHEEL
	};

public:
	PointerEvent(POINTER_EVENT_TYPE pet, int xPos, int yPos) :
		m_pet(pet), m_xPos(xPos), m_yPos(yPos)
	{}
	PointerEvent(POINTER_EVENT_TYPE pet, int xPos, int yPos, int wheelDelta) :
		m_pet(pet), m_xPos(xPos), m_yPos(yPos), m_wheelDelta(wheelDelta)
	{}

	inline POINTER_EVENT_TYPE GetType() const { return m_pet; }
	inline int GetXPos() const { return m_xPos; }
	inline int GetYPos() const { return m_yPos; }
	inline int GetWheelDelta() const { return m_wheelDelta; }

	std::string ToString() const;
private:
	POINTER_EVENT_TYPE m_pet;
	int m_xPos;
	int m_yPos;
	int m_wheelDelta;
};