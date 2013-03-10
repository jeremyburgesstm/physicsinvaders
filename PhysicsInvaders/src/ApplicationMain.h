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

// Need this for Windows typedefs unfortunately.
#include <Windows.h>

// STL
#include <list>

// Forward declarations
class RendererD3D;
class JBRocketSystemInterface;
class JBRocketRenderInterfaceD3D;
namespace Rocket {
	namespace Core {
		class Context;
	}
}
template<typename UpdateArgType>
class ThreadedStateMachine;
class ApplicationContext;

/**
 * \class ApplicationMain
 *
 * Simple class that acts as the root of the class structure for this
 * sample application. Construction initialises all windows procedure stuff,
 * DoMainLoop should be called once and it will perform game/windows loop,
 * and Destruction shuts down core windows systems.
 *
 * @author Jeremy Burgess
 */
class ApplicationMain
{
public:
	/// Construction of this class performs basic windows initialisation.
	ApplicationMain();

	/// Destruction of this class performs windows shutdown.
	~ApplicationMain();

	/// This method sits and spins invoking core game systems. 
	void DoMainLoop();

private:
	struct WinEvent
	{
		WinEvent() {}
		WinEvent(UINT msg, WPARAM wParam, LPARAM lParam) :
			m_msg(msg), m_wParam(wParam), m_lParam(lParam) {}

		UINT m_msg;
		WPARAM m_wParam;
		LPARAM m_lParam;
	};

private:
	/// Static windows message pump procedure.
	static LRESULT WINAPI StaticWindowsMessagePump( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	/// Non-static windows message pump procedure.
	LRESULT WINAPI WindowsMessagePump( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	/// Core update method (do any application level non-render related updates).
	void CoreUpdate();

	/// Synchronise update method (push any necessary data from core to rendered).
	void SynchroniseRenderData();

	/// Render update method (do any rendering that needs to be done).
	void RenderUpdate();

	/// Renders forever.
	void RenderLoop();

private:
	WNDCLASSEX wc;
	HWND m_hWnd;

	// Direct 3d Renderer. Note - this shouldn't actually live here.
	RendererD3D* m_d3dRenderer;
	// Rocket system interface.
	JBRocketSystemInterface* m_rocketSystemInterface;
	JBRocketRenderInterfaceD3D* m_rocketRenderInterface;
	Rocket::Core::Context* m_rocketContext;
	std::list<WinEvent> m_rocketEventQueue;
	// Our Flow Manager
	ThreadedStateMachine<const ApplicationContext*>* m_stateMachine;
};

