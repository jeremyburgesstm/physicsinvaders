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

#include "Win32InputState.h"
#include <WindowsX.h>

static std::vector<PointerEvent>* s_pointerEvents = nullptr;
static std::vector<KeyboardEvent>* s_keyEvents = nullptr;
static PointerState* s_pointerState = nullptr;
static KeyCodes::KeyCodeValue s_keyMap[256];
static bool s_keyStates[256];
static bool s_isActive = true;
static HWND s_hWnd;

void Win32InputState::Initialise(int maxX, int maxY, HWND hWnd)
{
	s_pointerEvents = new std::vector<PointerEvent>();
	s_keyEvents = new std::vector<KeyboardEvent>();
	s_pointerState = new PointerState();
	s_pointerState->SetMaxDims(maxX, maxY);
	s_hWnd = hWnd;
	SetActive(GetForegroundWindow() == hWnd);
	SetupKeyMap();
}

void Win32InputState::Cleanup()
{
	delete s_pointerEvents;
	delete s_pointerState;
}

const std::vector<PointerEvent>& Win32InputState::GetPointerEventQueue()
{
	return *s_pointerEvents;
}

const std::vector<KeyboardEvent>& Win32InputState::GetKeyboardEventQueue()
{
	return *s_keyEvents;
}

const PointerState& Win32InputState::GetCurrentPointerState()
{
	return *s_pointerState;
}

bool Win32InputState::GetKeyDown(KeyCodes::KeyCodeValue key)
{
	return s_keyStates[key];
}

bool Win32InputState::IsActive() 
{
	return s_isActive;
}

void Win32InputState::CleanEventQueue()
{
	s_pointerEvents->clear();
	s_keyEvents->clear();
}

void Win32InputState::ProcessWindowsEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_LBUTTONDOWN:
			s_pointerEvents->push_back(PointerEvent(PointerEvent::L_BUTTON_DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			s_pointerState->SetLeftDown(true);
			break;

		case WM_LBUTTONUP:
			s_pointerEvents->push_back(PointerEvent(PointerEvent::L_BUTTON_UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			s_pointerState->SetLeftDown(false);
			break;

		case WM_RBUTTONDOWN:
			s_pointerEvents->push_back(PointerEvent(PointerEvent::R_BUTTON_DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			s_pointerState->SetRightDown(true);
			break;

		case WM_RBUTTONUP:
			s_pointerEvents->push_back(PointerEvent(PointerEvent::R_BUTTON_UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			s_pointerState->SetRightDown(false);
			break;

		case WM_MBUTTONDOWN:
			s_pointerEvents->push_back(PointerEvent(PointerEvent::M_BUTTON_DOWN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			s_pointerState->SetMiddleDown(true);
			break;

		case WM_MBUTTONUP:
			s_pointerEvents->push_back(PointerEvent(PointerEvent::M_BUTTON_UP, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			s_pointerState->SetMiddleDown(false);
			break;

		case WM_MOUSEMOVE:
			s_pointerEvents->push_back(PointerEvent(PointerEvent::MOVE, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			s_pointerState->SetX(GET_X_LPARAM(lParam));
			s_pointerState->SetY(GET_Y_LPARAM(lParam));
			break;

		case WM_MOUSEWHEEL:
			s_pointerEvents->push_back(PointerEvent(PointerEvent::WHEEL, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_WHEEL_DELTA_WPARAM(wParam)));
			s_pointerState->AlterWheelPos(GET_WHEEL_DELTA_WPARAM(wParam));
			break;

		case WM_KEYDOWN:
			{
				KeyCodes::KeyCodeValue keycode = s_keyMap[wParam];
				s_keyEvents->push_back(KeyboardEvent(KeyboardEvent::KEY_DOWN, keycode));
				s_keyStates[keycode] = true;
			}
			break;

		case WM_KEYUP:
			{
				KeyCodes::KeyCodeValue keycode = s_keyMap[wParam];
				s_keyEvents->push_back(KeyboardEvent(KeyboardEvent::KEY_UP, keycode));
				s_keyStates[keycode] = false;
			}
			break;

		case WM_SIZE:
			s_pointerState->SetMaxDims(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_ACTIVATE:
			WORD val = LOWORD(wParam);
			if (val == WA_INACTIVE)
			{
				SetActive(false);
			}
			else if (val == WA_CLICKACTIVE || val == WA_ACTIVE)
			{
				SetActive(true);
			}
			break;
	}
}

void Win32InputState::SetActive(bool isActive)
{
	if (isActive)
	{
		// FOLLOWING CODE GOTTEN FROM MSDN
		// Get the window client area.
		RECT rc;
		GetClientRect(s_hWnd, &rc);

		// Convert the client area to screen coordinates.
		POINT pt = { rc.left, rc.top };
		POINT pt2 = { rc.right, rc.bottom };
		ClientToScreen(s_hWnd, &pt);
		ClientToScreen(s_hWnd, &pt2);
		SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);

		// Confine the cursor.
		ClipCursor(&rc);
	}
	else
	{
		ClipCursor(NULL);
	}
	s_isActive = isActive;
}

void Win32InputState::SetupKeyMap()
{
	// Zero the map & states
	memset(s_keyMap, 0, sizeof(s_keyMap));
	memset(s_keyStates, 0, sizeof(s_keyStates));

	// Fill in ASCII keys initially
	s_keyMap['A'] = KeyCodes::KC_A;
	s_keyMap['B'] = KeyCodes::KC_B;
	s_keyMap['C'] = KeyCodes::KC_C;
	s_keyMap['D'] = KeyCodes::KC_D;
	s_keyMap['E'] = KeyCodes::KC_E;
	s_keyMap['F'] = KeyCodes::KC_F;
	s_keyMap['G'] = KeyCodes::KC_G;
	s_keyMap['H'] = KeyCodes::KC_H;
	s_keyMap['I'] = KeyCodes::KC_I;
	s_keyMap['J'] = KeyCodes::KC_J;
	s_keyMap['K'] = KeyCodes::KC_K;
	s_keyMap['L'] = KeyCodes::KC_L;
	s_keyMap['M'] = KeyCodes::KC_M;
	s_keyMap['N'] = KeyCodes::KC_N;
	s_keyMap['O'] = KeyCodes::KC_O;
	s_keyMap['P'] = KeyCodes::KC_P;
	s_keyMap['Q'] = KeyCodes::KC_Q;
	s_keyMap['R'] = KeyCodes::KC_R;
	s_keyMap['S'] = KeyCodes::KC_S;
	s_keyMap['T'] = KeyCodes::KC_T;
	s_keyMap['U'] = KeyCodes::KC_U;
	s_keyMap['V'] = KeyCodes::KC_V;
	s_keyMap['W'] = KeyCodes::KC_W;
	s_keyMap['X'] = KeyCodes::KC_X;
	s_keyMap['Y'] = KeyCodes::KC_Y;
	s_keyMap['Z'] = KeyCodes::KC_Z;

	// Numbers
	s_keyMap['0'] = KeyCodes::KC_0;
	s_keyMap['1'] = KeyCodes::KC_1;
	s_keyMap['2'] = KeyCodes::KC_2;
	s_keyMap['3'] = KeyCodes::KC_3;
	s_keyMap['4'] = KeyCodes::KC_4;
	s_keyMap['5'] = KeyCodes::KC_5;
	s_keyMap['6'] = KeyCodes::KC_6;
	s_keyMap['7'] = KeyCodes::KC_7;
	s_keyMap['8'] = KeyCodes::KC_8;
	s_keyMap['9'] = KeyCodes::KC_9;

	// Useful keys
	s_keyMap[VK_ESCAPE] = KeyCodes::KC_ESCAPE;
	s_keyMap[VK_SUBTRACT] = KeyCodes::KC_SUBTRACT;
	s_keyMap[VK_ADD] = KeyCodes::KC_ADD;
	s_keyMap[VK_BACK] = KeyCodes::KC_BACK;
	s_keyMap[VK_DIVIDE] = KeyCodes::KC_DIVIDE;
	s_keyMap[VK_MULTIPLY] = KeyCodes::KC_MULTIPLY;
	s_keyMap[VK_TAB] = KeyCodes::KC_TAB;
	s_keyMap[VK_OEM_4] = KeyCodes::KC_LBRACKET;
	s_keyMap[VK_OEM_6] = KeyCodes::KC_RBRACKET;
	s_keyMap[VK_OEM_5] = KeyCodes::KC_BACKSLASH;
	s_keyMap[VK_OEM_1] = KeyCodes::KC_SEMICOLON;
	s_keyMap[VK_OEM_7] = KeyCodes::KC_QUOTE;
	s_keyMap[VK_OEM_3] = KeyCodes::KC_GRAVE;
	s_keyMap[VK_LSHIFT] = KeyCodes::KC_LSHIFT;
	s_keyMap[VK_OEM_COMMA] = KeyCodes::KC_COMMA;
	s_keyMap[VK_OEM_PERIOD] = KeyCodes::KC_PERIOD;
	s_keyMap[VK_OEM_2] = KeyCodes::KC_FORWARDSLASH;
	s_keyMap[VK_RSHIFT] = KeyCodes::KC_RSHIFT;
	s_keyMap[VK_RETURN] = KeyCodes::KC_RETURN;
	s_keyMap[VK_LCONTROL] = KeyCodes::KC_LCONTROL;
	s_keyMap[VK_LMENU] = KeyCodes::KC_LALT;
	s_keyMap[VK_RMENU] = KeyCodes::KC_RALT;
	s_keyMap[VK_RCONTROL] = KeyCodes::KC_RCONTROL;
	s_keyMap[VK_LEFT] = KeyCodes::KC_LEFT;
	s_keyMap[VK_RIGHT] = KeyCodes::KC_RIGHT;
	s_keyMap[VK_UP] = KeyCodes::KC_UP;
	s_keyMap[VK_DOWN] = KeyCodes::KC_DOWN;

	// TODO: Fill in all the other keys we care support.

}