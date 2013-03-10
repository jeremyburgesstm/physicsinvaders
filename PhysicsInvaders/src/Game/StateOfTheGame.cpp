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
#include "StateOfTheGame.h"

// Project headers we require.
#include "Core/Functional/Action.h"
#include "Messaging/GameMessageHub.h"
#include "ComponentModel/Entity.h"
#include "Components/Invader.h"
#include "Components/Bullet.h"

StateOfTheGame::StateOfTheGame(unsigned int totalLevels, unsigned int lives, GameMessageHub& messageHub) :
	m_totalLevels(totalLevels),
	m_currentLevel(0),
	m_combo(1),
	m_score(0),
	m_bestScoreThisSession(0),
	m_lives(lives),
	m_totalLives(lives),
	m_messageHub(messageHub)
{
	m_messageHub.SubscribeGameEvent(GameEventTypes::GE_ENTITY_DESTROYED, Functional::Creator::CreateAction(this, &StateOfTheGame::OnEntityDestroyed));
	m_messageHub.SubscribeGameEvent(GameEventTypes::GE_PLAYER_LOST_LIFE, Functional::Creator::CreateAction(this, &StateOfTheGame::OnPlayerLostLife));
}

StateOfTheGame::~StateOfTheGame()
{
	m_messageHub.UnsubscribeGameEvent(GameEventTypes::GE_ENTITY_DESTROYED, Functional::Creator::CreateAction(this, &StateOfTheGame::OnEntityDestroyed));
	m_messageHub.UnsubscribeGameEvent(GameEventTypes::GE_PLAYER_LOST_LIFE, Functional::Creator::CreateAction(this, &StateOfTheGame::OnPlayerLostLife));
}

void StateOfTheGame::IncrementCurrentLevel()
{
	++m_currentLevel;
	m_combo = 1;
}

void StateOfTheGame::ResetStateOfTheGame()
{
	m_currentLevel = 0;
	m_combo = 1;
	m_score = 0;
	m_lives = m_totalLives;
}

void StateOfTheGame::OnEntityDestroyed(GameEventTypes::GameEvent /*event*/, ComponentModel::Entity* entity)
{
	// If the entity is an invader:
	Invader* invader = entity->GetComponentByTypeFast<Invader>();
	if (invader != nullptr)
	{
		ProcessInvaderDeath(invader);
		return;
	}

	// If the entity is a bullet:
	Bullet* bullet = entity->GetComponentByTypeFast<Bullet>();
	if (bullet != nullptr)
	{
		ProcessBulletDeath(bullet);
		return;
	}
}

void StateOfTheGame::OnPlayerLostLife(GameEventTypes::GameEvent /*event*/, ComponentModel::Entity* /*entity*/)
{
	// When the player loses a life, zero the combo.
	m_combo = 1;
	--m_lives;
}

void StateOfTheGame::ProcessInvaderDeath(Invader* invader)
{
	switch (invader->GetDeathCause())
	{
	case Invader::DC_BULLET:
		m_score += invader->GetConfig().m_baseScore * m_combo;
		m_combo += invader->GetConfig().m_multiplierChange;
		break;
	case Invader::DC_FLOOR:
		m_score += invader->GetConfig().m_baseScore * m_combo;
		break;
	}
}

void StateOfTheGame::ProcessBulletDeath(Bullet* bullet)
{
	// If the player missed, zero their combo.
	if (bullet->GetOwnerType() == Bullet::BOT_PLAYER 
		&& bullet->GetHitType() == Bullet::BHT_OUT_OF_BOUNDS)
	{
		m_combo = 1;
	}
}