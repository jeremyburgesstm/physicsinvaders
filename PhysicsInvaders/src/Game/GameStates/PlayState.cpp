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
#include "PlayState.h"

// STL
#include <algorithm>

// Game state headers
#include "GameStateContext.h"

// Core headers
#include "Core/Functional/Action.h"
#include "Core/GameTime.h"
#include "Core/ITimeSource.h"
#include "Input/InputSystem.h"
#include "Utility/Helpers.h"

// Game headers
#include "Game/GameContext.h"
#include "Game/StateOfTheGame.h"
#include "Game/Messaging/GameMessageHub.h"
#include "Game/Screens/HUDScreen.h"

void PlayState::UpdateState(const GameStateContext* context)
{
	HUDScreen& hud = context->GetHud();
	StateOfTheGame& sotg = context->GetStateOfTheGame();
	float scoreDiff = sotg.GetScore() - m_lastScore;
	float rate = (float)(Helpers::Clamp(scoreDiff, (float)c_minScoreForEval, (float)c_maxScoreForEval) - (float)c_minScoreForEval) / (float)(c_maxScoreForEval - c_minScoreForEval);
	rate = rate * rate;
	rate = context->GetGameTime().GetGameTime()->GetFrameDeltaSeconds() * (rate * (float)(c_maxMoveRate - c_minMoveRate) + c_minMoveRate);
	m_lastScore = Helpers::Min((float)sotg.GetScore(), m_lastScore + rate);
	hud.SetScore((int)m_lastScore);
	hud.SetCombo(sotg.GetCombo());
	hud.SetNumberOfLives(sotg.GetNumLives());
}

void PlayState::OnEnter(const GameStateContext* context)
{
	// Register for events.
	m_lastScore = context->GetStateOfTheGame().GetScore() < m_lastScore ? context->GetStateOfTheGame().GetScore() : m_lastScore;
	context->GetGameContext().GetMessageHub().SubscribeGameEvent(GameEventTypes::GE_ALL_INVADERS_DESTROYED, Functional::Creator::CreateAction(this, &PlayState::OnPlayerWonEvent));
	context->GetGameContext().GetMessageHub().SubscribeGameEvent(GameEventTypes::GE_PLAYER_DEFEATED, Functional::Creator::CreateAction(this, &PlayState::OnPlayerLostEvent));
}

void PlayState::OnExit(const GameStateContext* context)
{
	// Unregister for events.
	context->GetGameContext().GetMessageHub().UnsubscribeGameEvent(GameEventTypes::GE_ALL_INVADERS_DESTROYED, Functional::Creator::CreateAction(this, &PlayState::OnPlayerWonEvent));
	context->GetGameContext().GetMessageHub().UnsubscribeGameEvent(GameEventTypes::GE_PLAYER_DEFEATED, Functional::Creator::CreateAction(this, &PlayState::OnPlayerLostEvent));
}

void PlayState::OnPlayerWonEvent(GameEventTypes::GameEvent, ComponentModel::Entity*)
{
	SetPendingExit(STATE_EXIT_PLAYER_WON);
}

void PlayState::OnPlayerLostEvent(GameEventTypes::GameEvent, ComponentModel::Entity*)
{
	SetPendingExit(STATE_EXIT_PLAYER_LOST);
}