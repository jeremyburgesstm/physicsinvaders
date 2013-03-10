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
#include "EndState.h"

// Game state headers
#include "GameStateContext.h"

// Headers we need to use.
#include "Core/GameTime.h"
#include "Rocket/ProgressScreen.h"
#include "Game/Screens/HUDScreen.h"
#include "Game/StateOfTheGame.h"
#include <Rocket/Core.h>

void EndState::OnEnter(const GameStateContext* context)
{
	context->GetHud().Hide();
	ProgressScreen& screen = GetScreen(context);
	screen.Show();
	screen.ClearReadyToExit();
	screen.SetTextOnElement("score", Rocket::Core::String(24, "SCORE: %d", context->GetStateOfTheGame().GetScore()));
	context->GetGameTime().SetTimeScale(0);
}

void EndState::OnExit(const GameStateContext* context)
{
	context->GetHud().Show();
	GetScreen(context).Hide();
	context->GetGameTime().SetTimeScale(1);
}

void EndState::UpdateState(const GameStateContext* context)
{
	if (GetScreen(context).GetReadyToExit())
	{
		SetPendingExit(0);
	}
}

ProgressScreen& EndState::GetScreen(const GameStateContext* context) const
{
	return m_victory ? context->GetVictory() : context->GetDefeat();
}