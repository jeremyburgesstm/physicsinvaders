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

// Forward declarations
class GameMessageHub;

// Box2D Base Class
#include <Box2D/Dynamics/b2WorldCallbacks.h>

/**
 * Our implmentation of the Box2D collision event listener. Talks directly
 * to the game message hub. Seperated out so that the game message hub itself
 * does not need to listen to the box 2d world directly.
 */
class Box2DMessageListener : public b2ContactListener
{
public:
	Box2DMessageListener(GameMessageHub* messageHub);

	/// \name b2ContactListener members
	/// @{
		virtual void BeginContact(b2Contact* contact);
		virtual void EndContact(b2Contact* contact);
	/// @}

private:
	GameMessageHub* m_messageHub;
};