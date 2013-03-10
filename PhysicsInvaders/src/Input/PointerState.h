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

class PointerState
{
public:
	PointerState() :
		m_xPosition(0), m_yPosition(0), m_wheelPosition(0),
		m_leftButtonDown(false), m_rightButtonDown(false), m_middleButtonDown(false)
	{}

	inline int GetX() const { return m_xPosition; }
	inline void SetX(int x) { m_xPosition = x; }
	inline int GetY() const { return m_yPosition; }
	inline void SetY(int y) { m_yPosition = y; }
	inline void SetMaxDims(int x, int y) { m_screenXMax = x; m_screenYMax = y; }
	inline float GetXNormalised() const { return (float)m_xPosition/m_screenXMax; }
	inline float GetYNormalised() const { return (float)m_yPosition/m_screenYMax; }
	inline int GetXMax() const { return m_screenXMax; }
	inline int GetYMax() const { return m_screenYMax; }
	inline int GetWheelPos() const { return m_wheelPosition; }
	inline void SetWheelPos(int wheel) { m_wheelPosition = wheel; }
	inline void AlterWheelPos(int var) { m_wheelPosition += var; }
	inline bool GetLeftDown() const { return m_leftButtonDown; }
	inline void SetLeftDown(bool down) { m_leftButtonDown = down; }
	inline bool GetRightDown() const { return m_rightButtonDown; }
	inline void SetRightDown(bool down) { m_rightButtonDown = down; }
	inline bool GetMiddleDown() const { return m_middleButtonDown; }
	inline void SetMiddleDown(bool down) { m_middleButtonDown = down; }
	inline bool GetPositionValid() const { return m_screenXMax > 0 && m_screenYMax > 0; }
	
	std::string ToString() const;

private:
	int m_xPosition;
	int m_yPosition;
	int m_screenXMax;
	int m_screenYMax;
	int m_wheelPosition;
	bool m_leftButtonDown;
	bool m_rightButtonDown;
	bool m_middleButtonDown;
};