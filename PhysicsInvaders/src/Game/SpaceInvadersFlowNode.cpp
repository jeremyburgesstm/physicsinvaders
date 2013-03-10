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

// My Header
#include "SpaceInvadersFlowNode.h"

// Other headers.
#include "Graphics/RendererD3D.h"
#include "GameWorld.h"
#include "Game/Screens/HUDScreen.h"
#include "Rocket/ProgressScreen.h"

SpaceInvadersFlowNode::SpaceInvadersFlowNode(const RendererD3D& renderer, 
		Rocket::Core::Context& rocketContext, 
		const char* pathToHud,		
		const char* pathToVictory,
		const char* pathToDefeat) :
	ThreadedBaseState<const ApplicationContext*>(), 
	m_renderer(renderer),
	m_gameWorld(nullptr),
	m_rocketContext(rocketContext)
{ 
	m_hud = new HUDScreen(rocketContext, pathToHud);
	m_victory = new ProgressScreen(rocketContext, pathToVictory, "clickTarget", "click");
	m_defeat = new ProgressScreen(rocketContext, pathToDefeat, "clickTarget", "click");
}

SpaceInvadersFlowNode::~SpaceInvadersFlowNode(void)
{
	delete m_hud;
	delete m_victory;
	delete m_defeat;
}

void SpaceInvadersFlowNode::CoreUpdate(const ApplicationContext*)
{
	m_gameWorld->CoreUpdate();
}

void SpaceInvadersFlowNode::RenderUpdate(const ApplicationContext*)
{
	m_gameWorld->RenderUpdate();
}

void SpaceInvadersFlowNode::SynchroniseRenderData(const ApplicationContext*)
{
	m_gameWorld->SynchroniseRenderData();
}

void SpaceInvadersFlowNode::OnEnter(const ApplicationContext*)
{
	m_gameWorld = new GameWorld(m_renderer, *m_hud, *m_victory, *m_defeat);
}

void SpaceInvadersFlowNode::OnExit(const ApplicationContext*)
{
	m_hud->Hide();
	m_defeat->Hide();
	m_victory->Hide();
	delete m_gameWorld;
}
