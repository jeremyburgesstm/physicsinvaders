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

#include "ApplicationSetup.h"

#include "Core/StateMachine/ThreadedStateMachine.h"
#include "Screens/RocketFlowScreenTimed.h"
#include "Screens/RocketFlowScreenClickBody.h"
#include "Game/SpaceInvadersFlowNode.h"

#include <Rocket/Core.h>

void ShouldBeDataDriven::InitiliaseFlow(Rocket::Core::Context& context, ThreadedStateMachine<const ApplicationContext*>* manager, const RendererD3D& renderer)
{
	LoadFonts();

	RocketFlowScreenClickBody* title = new RocketFlowScreenClickBody(
		context, "Content/UI/Intro/title.rml", "clickme", "click");
	RocketFlowScreenClickBody* pressStart = new RocketFlowScreenClickBody(
		context, "Content/UI/Intro/pressstart.rml", "clickme", "click");
	SpaceInvadersFlowNode* theGame = new SpaceInvadersFlowNode(renderer, context, "Content/UI/Game/hud.rml", "Content/UI/Game/victory.rml", "Content/UI/Game/defeat.rml");
	manager->AddState(title, true);
	manager->AddState(pressStart);
	manager->AddState(theGame);
	manager->ConnectStates(title, pressStart, 0);
	manager->ConnectStates(pressStart, theGame, 0);
}

void ShouldBeDataDriven::LoadFonts()
{
	Rocket::Core::String font_names[4];
	font_names[0] = "Delicious-Roman.otf";
	font_names[1] = "Delicious-Italic.otf";
	font_names[2] = "Delicious-Bold.otf";
	font_names[3] = "Delicious-BoldItalic.otf";

	for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
	{
		Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String("Content/UI/Fonts/") + font_names[i]);
	}
}

