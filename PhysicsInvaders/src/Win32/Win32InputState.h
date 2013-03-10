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

#include <windows.h>
#include <vector>
#include "Input/PointerEvent.h"
#include "Input/PointerState.h"
#include "Input/KeyCodes.h"
#include "Input/KeyboardEvent.h"

class Win32InputState
{
public:
	static void Initialise(int width, int height, HWND hwnd);
	static void Cleanup();

	static void CleanEventQueue();

	/// Process the Windows message.
	static void ProcessWindowsEvent(UINT message, WPARAM w_param, LPARAM l_param);

	static const std::vector<PointerEvent>& GetPointerEventQueue();
	static const std::vector<KeyboardEvent>& GetKeyboardEventQueue();
	static const PointerState& GetCurrentPointerState();
	static bool GetKeyDown(KeyCodes::KeyCodeValue key);
	static bool IsActive();

private:
	static void SetActive(bool isActive);
	static void SetupKeyMap();
};

