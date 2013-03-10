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
#include "HUDScreen.h"

// Rocket
#include <Rocket/Core.h>

HUDScreen::HUDScreen(Rocket::Core::Context& rocketContext,
		const char* pathToDocument) :
	RocketScreen(rocketContext, pathToDocument),
	m_lastScore(INT_MIN),
	m_lastCombo(INT_MIN),
	m_lastLives(INT_MIN)
{
	m_scoreElement = (Rocket::Core::ElementText*)m_rocketDocument->GetElementById("score")->GetFirstChild();
	m_livesElement = (Rocket::Core::ElementText*)m_rocketDocument->GetElementById("lives")->GetFirstChild();
	m_comboElement = (Rocket::Core::ElementText*)m_rocketDocument->GetElementById("combo")->GetFirstChild();
	m_titleElement = m_rocketDocument->GetElementById("title");
	m_titleText = (Rocket::Core::ElementText*)m_titleElement->GetFirstChild();
}

void HUDScreen::SetScore(int score)
{
	if (score != m_lastScore)
	{
		m_lastScore = score;
		m_scoreElement->SetText(Rocket::Core::String(16, "%d", score));
	}
}

void HUDScreen::SetNumberOfLives(int lives)
{
	if (m_lastLives != lives)
	{
		m_lastLives = lives;
		m_livesElement->SetText(Rocket::Core::String(4, "%d", lives));
	}
}

void HUDScreen::SetCombo(int combo)
{
	if (m_lastCombo != combo)
	{
		m_lastCombo = combo;
		m_comboElement->SetText(Rocket::Core::String(6, "%d", combo));
	}
}

void HUDScreen::ShowTitle(const std::string& title)
{
	m_titleElement->SetProperty("visibility", "visible");
	m_titleText->SetText(title.c_str());
}

void HUDScreen::HideTitle()
{
	m_titleElement->SetProperty("visibility", "hidden");
}