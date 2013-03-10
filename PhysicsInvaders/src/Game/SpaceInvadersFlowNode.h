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

// Base header
#include "Core/StateMachine/ThreadedBaseState.h"

// Boost inheritance
#include <boost/noncopyable.hpp>

// STL
#include <string>

// Forward declarations
class GameWorld;
class RendererD3D;
class ApplicationContext;
class HUDScreen;
class ProgressScreen;
namespace Rocket {
	namespace Core {
		class Context;
		class ElementDocument;
	}
}

class SpaceInvadersFlowNode :
	public ThreadedBaseState<const ApplicationContext*>,
	public boost::noncopyable
{
public:
	SpaceInvadersFlowNode(const RendererD3D& renderer, 
		Rocket::Core::Context& rocketContext, 
		const char* pathToHud,
		const char* pathToVictory,
		const char* pathToDefeat);
	virtual ~SpaceInvadersFlowNode(void);
	
	unsigned char GetExitCount() const { return 1; }

	virtual void CoreUpdate(const ApplicationContext*);
	virtual void RenderUpdate(const ApplicationContext*);
	virtual void SynchroniseRenderData(const ApplicationContext*);

protected:
	virtual void OnEnter(const ApplicationContext*);
	virtual void OnExit(const ApplicationContext*);

private:
	// The physics/gameworld for this instance of space invaders.
	GameWorld* m_gameWorld;

	// Reference for renderer
	const RendererD3D& m_renderer;

	Rocket::Core::Context& m_rocketContext;
	HUDScreen* m_hud;
	ProgressScreen* m_victory;
	ProgressScreen* m_defeat;
};

