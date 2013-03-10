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
#include "ApplicationMain.h"

// STL includes
#include <iostream>

// Boost
#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

// Required includes
#include "Graphics/RendererD3D.h"
#include "Utility/Log.h"
#include "Utility/LogDebugTarget.h"
#include "Utility/ApplicationTime.h"
#include "Core/StateMachine/ThreadedStateMachine.h"
#include "Core/RunInformation.h"
#include "Core/ConfigFile.h"
#include "Win32/Win32InputState.h"
#include "ShouldBeDataDriven/ApplicationSetup.h"

// Rocket local includes / temp input handling
#include "Rocket/JBRocketSystemInterface.h"
#include "Rocket/JBRocketRenderInterfaceD3D.h"
#include "Rocket/InputWin32.h"

// LibRocket
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>

#define ENABLE_MULTITHREADED_RENDERING

const char APPLICATION_NAME[] = "JBSample";
const char APPLICATION_TITLE[] = "Jeremy Burgess Sample";
#ifdef ENABLE_MULTITHREADED_RENDERING
boost::interprocess::interprocess_semaphore renderSemaphore(1);
boost::interprocess::interprocess_semaphore updateSemaphore(0);
#endif
ApplicationMain* s_applicationMainPtr;

ApplicationMain::ApplicationMain(void)
{	
	// Initialise application time.
	ApplicationTime::InitialiseGlobalTime();

	// Initialise the crude logging system.
	Log::Initialise();

	// This should be initialised from data, but for now it will do.
	for (Log::Constants::Channel i = Log::Constants::CHANNEL_NONE;
		i < Log::Constants::CHANNEL_COUNT; i = (Log::Constants::Channel)(i + 1))
	{
		for (Log::Constants::Level j = Log::Constants::LEVEL_NONE;
			j < Log::Constants::LEVEL_COUNT; j = (Log::Constants::Level)(j + 1))
		{
			Log::RegisterLogTarget(i, j, new Log::LogDebugTarget());
		}
	}

	// Config variables for initialising the application
	ConfigFile config("Content/config.txt");
	bool fullscreen = config.GetFullscreen();
	unsigned short width = config.GetScreenWidth();
	unsigned short height = config.GetScreenHeight();
	if (fullscreen)
	{
		width = (unsigned short)GetSystemMetrics(SM_CXSCREEN);
		height = (unsigned short)GetSystemMetrics(SM_CYSCREEN);
	}

	RunInformation::SetScreenDimensions(width, height);

    // Register the window class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = ApplicationMain::StaticWindowsMessagePump;
	wc.cbClsExtra = 0L;
	wc.cbWndExtra = 0L;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "JBSample";
	wc.hIconSm = NULL;

	RegisterClassEx( &wc );

	RECT r={0, 0, width, height};
	
	int style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	style|=WS_VISIBLE;

	AdjustWindowRect(&r,style,false);

	s_applicationMainPtr = this;

    // Create the application's window
	m_hWnd = CreateWindow( "JBSample", "Jeremy Burgess Sample",
		style, CW_USEDEFAULT,CW_USEDEFAULT,r.right-r.left,r.bottom-r.top,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	// Create the renderer
	m_d3dRenderer = new RendererD3D(m_hWnd);

	// Create our input state
	Win32InputState::Initialise(width, height, m_hWnd);

	// Initialise LibRocket
	m_rocketSystemInterface = new JBRocketSystemInterface();
	Rocket::Core::SetSystemInterface(m_rocketSystemInterface);
	m_rocketRenderInterface = new JBRocketRenderInterfaceD3D(m_d3dRenderer, "Content/Shaders/TexturedUnlit.fx");
	Rocket::Core::SetRenderInterface(m_rocketRenderInterface);
	RocketHelp::InputWin32::Initialise();
	Rocket::Core::Initialise();
	m_rocketContext = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(width, height));
	Rocket::Debugger::Initialise(m_rocketContext);
	Input::SetContext(m_rocketContext);
	
	// Create our flow manager.
	m_stateMachine = new ThreadedStateMachine<const ApplicationContext*>();
	ShouldBeDataDriven::InitiliaseFlow(*m_rocketContext, m_stateMachine, *m_d3dRenderer);

	// Show the window
	ShowWindow( m_hWnd, SW_SHOWDEFAULT );
	UpdateWindow( m_hWnd );
}

ApplicationMain::~ApplicationMain(void)
{	
	// Delete the application flow manager.
	delete m_stateMachine;

	// Shutdown Rocket.
	m_rocketContext->RemoveReference();
	Rocket::Core::Shutdown();
	RocketHelp::InputWin32::Shutdown();
	
	Rocket::Core::SetSystemInterface(NULL);
	delete m_rocketSystemInterface;

	Rocket::Core::SetRenderInterface(NULL);
	delete m_rocketRenderInterface;

	// Shutdown our win32 input handler
	Win32InputState::Cleanup();

	// Destroy the renderer
	delete m_d3dRenderer;
	
	// Shutdown logging.
	Log::Cleanup();
	
	// Shutdown core time source.
	ApplicationTime::ShutdownGlobalTime();

	UnregisterClass( "JBSample", wc.hInstance );
}

LRESULT WINAPI ApplicationMain::StaticWindowsMessagePump( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// Defer to proper OO logic.
	return s_applicationMainPtr->WindowsMessagePump( hWnd, msg, wParam, lParam );
}

LRESULT WINAPI ApplicationMain::WindowsMessagePump( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
    {
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
		break;
	default:
		m_rocketEventQueue.push_back(WinEvent(msg, wParam, lParam));
		Win32InputState::ProcessWindowsEvent(msg, wParam, lParam);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void ApplicationMain::DoMainLoop()
{
	bool running = true;
#ifdef ENABLE_MULTITHREADED_RENDERING
	// If multithreading is enabled, first update the core stuff once, so that the
	// render data is guaranteed to be populated
	CoreUpdate();
	SynchroniseRenderData();

	// Now create a semaphore, and a thread to represent the renderer
	boost::thread renderThread([this](){this->RenderLoop();});
#endif
	while (running)
	{
		// Clear the input event queue before starting.
		Win32InputState::CleanEventQueue();
		MSG message;
		while( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ))
		{
			// Get out if the message is a quit message.
			if (message.message == WM_QUIT) return;
			
			// Otherwise, deal with it in our loop.
			TranslateMessage( &message );
			DispatchMessage( &message );
		}

#ifndef ENABLE_MULTITHREADED_RENDERING
		CoreUpdate();
		SynchroniseRenderData();
		RenderUpdate();
#else
		CoreUpdate();
		renderSemaphore.wait();
		SynchroniseRenderData();
		updateSemaphore.post();
#endif
	}
	renderThread.join();
}

void ApplicationMain::RenderLoop()
{
	bool running = true;
	while (running)
	{
		updateSemaphore.wait();
		RenderUpdate();
		renderSemaphore.post();
	}
}

void ApplicationMain::CoreUpdate()
{
	// Update the flow manager.
	m_stateMachine->CoreUpdate(nullptr);
}

void ApplicationMain::SynchroniseRenderData()
{
	// Update the rocket context. Rocket can't be run multithreaded
	// safely, so we just serialise the update and render by putting the
	// update here.
	while (!m_rocketEventQueue.empty())
	{
		const WinEvent& e = m_rocketEventQueue.front();
		RocketHelp::InputWin32::ProcessWindowsEvent(e.m_msg, e.m_wParam, e.m_lParam);
		m_rocketEventQueue.pop_front();
	}
	m_rocketContext->Update();
	// Synchronise the flow manager.
	m_stateMachine->SynchroniseRenderData(nullptr);
}

void ApplicationMain::RenderUpdate()
{
	// Clear the back buffer
	m_d3dRenderer->Clear(0,0,0,0);

	// DRAW THE GAME.
	m_stateMachine->RenderUpdate(nullptr);
	m_rocketContext->Render();

	// Flip buffers
	m_d3dRenderer->FlipBuffers();
}