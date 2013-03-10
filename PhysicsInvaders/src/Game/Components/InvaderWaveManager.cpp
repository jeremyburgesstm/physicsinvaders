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
#include "InvaderWaveManager.h"

// Project headers
#include "Invader.h"
#include "InvaderWaveMover.h"
#include "Core/Functional/Action.h"
#include "Core/GameTime.h"
#include "Core/ScaledTime.h"
#include "Game/GameContext.h"
#include "Game/Messaging/GameMessageHub.h"
#include "Utility/GenericTraversal.h"
#include "Utility/Helpers.h"
#include "Utility/ApplicationTime.h"

// STL
#include <random>

using namespace ComponentModel;

// Setup a random number engine for use here.
std::tr1::minstd_rand generator;

InvaderWaveManager::InvaderWaveManager() :
	m_recalculatePower(true)
{}

void InvaderWaveManager::Initialise(const GameContext& gameContext)
{
	// Get cached items.
	m_recalculatePower = true;
	
	// Register for events.
	gameContext.GetMessageHub().SubscribeGameEvent(GameEventTypes::GE_ENTITY_DESTROYED, Functional::Creator::CreateAction(this, &InvaderWaveManager::OnEntityDestroyedEvent));

	// Cache invader wave mover pointer.
	m_mover = m_entity->GetComponentByTypeFast<InvaderWaveMover>();

	// Zero number of dead invaders
	m_numberOfDeadInvaders = 0;
	m_numberOfPoweredInvaders = 0;
}

void InvaderWaveManager::Cleanup(const GameContext& gameContext)
{
	// Unregister all events.
	gameContext.GetMessageHub().UnsubscribeGameEvent(GameEventTypes::GE_ENTITY_DESTROYED, Functional::Creator::CreateAction(this, &InvaderWaveManager::OnEntityDestroyedEvent)); 

	// Clear data.
	m_entityInvaders.clear();

	// Mover = null
	m_mover = nullptr;
}

void InvaderWaveManager::FirstCoreUpdate(const GameTime& time, const GameContext& /*gameContext*/)
{
	m_timeLastFired = time.GetGameTime()->GetCurrentTime();
	generator.seed(time.GetRealTime()->GetCurrentTime());
}

void InvaderWaveManager::CoreUpdate(const GameTime& time, const GameContext& gameContext)
{
	if (m_recalculatePower)
	{
		if (m_entityInvaders.size() > 0)
		{
			UpdatePoweredStatus();
		}
		else
		{
			gameContext.GetMessageHub().RaiseGameEvent(GameEventTypes::GE_ALL_INVADERS_DESTROYED, m_entity);
			m_entity->Destroy();
			m_recalculatePower = false;
		}
	}

	// Always recalculate difficulty.
	m_currentDifficulty = Helpers::Clamp<float>(
		m_config.m_baseDifficulty + m_config.m_difficultyIncrementPerInvaderDeath * m_numberOfDeadInvaders, 
		0, 1);
	// Set it on the mover.
	m_mover->SetDifficulty(m_currentDifficulty);

	// Determine if it's time to shoot.
	float currentFireRate = GetCurrentFireRate();
	if (currentFireRate > 0)
	{
		float secondsToShoot = 1 / currentFireRate;
		if (ApplicationTime::ConvertTicksToSeconds(time.GetGameTime()->GetCurrentTime() - m_timeLastFired) > secondsToShoot)
		{			
			// Pick a random invader, tell them to fire.
			std::uniform_int_distribution<size_t> distribution(0, m_numberOfPoweredInvaders - 1);
			size_t invaderIndex = distribution(generator);
			size_t count = 0;
			for (auto invIt = m_entityInvaders.begin(); invIt != m_entityInvaders.end(); ++invIt)
			{
				Invader* invader = (*invIt).second;
				if (invader->GetPowered())
				{
					if (count == invaderIndex)
					{
						invader->Fire();
						m_timeLastFired = time.GetGameTime()->GetCurrentTime();
						break;
					}
					++count;
				}
			}
		}
	}
}

void InvaderWaveManager::AddInvader(Entity* invaderEntity)
{
	m_entityInvaders[invaderEntity] = invaderEntity->GetComponentByTypeFast<Invader>();
}

void InvaderWaveManager::UpdatePoweredStatus()
{
	// First set all invaders to not be powered.
	m_numberOfPoweredInvaders = 0;
	for (auto invIt = m_entityInvaders.begin(); invIt != m_entityInvaders.end(); ++invIt)
	{
		(*invIt).second->SetPowered(false);
	}

	// Now perform a 'depth first search' on all of them to power them up.
	// Any that are not visited are no longer powered!
	for (auto invIt = m_entityInvaders.begin(); invIt != m_entityInvaders.end(); ++invIt)
	{
		// Only start from 'root' nodes.
		if ((*invIt).second->GetConnectedToRoot())
		{
			Traversal::DepthFirstPreRecurse(
				(*invIt).second,
				&Invader::GetConnectedEntityCount,
				&Invader::GetConnectedEntity,
				[](Invader* i) { return !i->GetPowered(); },
				[this](Invader* i) 
				{ 
					i->SetPowered(true); 
					m_numberOfPoweredInvaders++;
				} );
		}
	}

	// Clear the recompute flag
	m_recalculatePower = false;
}

void InvaderWaveManager::OnEntityDestroyedEvent(GameEventTypes::GameEvent /*event*/, ComponentModel::Entity* entity)
{
	// We only see destroyed events, so ignore entities which are irrelevant.
	if (m_entityInvaders.count(entity) > 0)
	{
		m_recalculatePower = true;
		m_entityInvaders.erase(entity);
		++m_numberOfDeadInvaders;
	}
}

float InvaderWaveManager::GetCurrentFireRate() const
{
	return Helpers::Lerp(m_config.m_minFireRate, m_config.m_maxFireRate, m_currentDifficulty);
}

