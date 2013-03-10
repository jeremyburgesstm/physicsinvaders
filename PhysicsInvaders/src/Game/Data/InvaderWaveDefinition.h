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

#include <vector>

/// Data class which defines an invader
class InvaderDefinition
{
public:
	int m_id;
	int m_invaderType;
	float m_xOffset;
	float m_yOffset;
};

class InvaderConnectionDefinition
{
public:
	InvaderConnectionDefinition(int firstID, int secondID) : 
		m_firstID(firstID),
		m_secondID(secondID)
	{}

	int m_firstID;
	int m_secondID;
};

class InvaderRootConnectionDefinition
{
public:
	InvaderRootConnectionDefinition(int invaderID, float xConnectionPosition) :
		m_invaderID(invaderID),
		m_xConnectionPosition(xConnectionPosition)
	{}

	int m_invaderID;
	float m_xConnectionPosition;
};

/// Data class which defines the structure of an invader wave.
class InvaderWaveDefinition
{
public:
	std::vector<InvaderDefinition> m_invaders;
	std::vector<InvaderConnectionDefinition> m_invaderConnections;
	std::vector<InvaderRootConnectionDefinition> m_invaderRootConnections;
	float m_difficulty;
};

