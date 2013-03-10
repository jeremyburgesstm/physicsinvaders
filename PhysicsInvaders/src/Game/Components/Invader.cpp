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
#include "Invader.h"

// Project headers
#include "Bullet.h"
#include "CoreComponents/Box2DBodyComponent.h"
#include "CoreComponents/MoveableQuadComponent.h"
#include "Core/Functional/Action.h"
#include "Core/GameTime.h"
#include "Game/GameContext.h"
#include "Game/Messaging/PhysicsContactEvent.h"
#include "Game/Messaging/GameMessageHub.h"
#include "Game/Physics/GamePhysicsConstants.h"
#include "Utility/Helpers.h"
#include "ShouldBeDataDriven/EntityCreation.h"

// Box 2D
#include <Box2D/Box2D.h>

Invader::Invader() :
	m_body(nullptr),
	m_config(0, 10, 0, 0),
	m_isPowered(false),
	m_isConnectedToRoot(false),
	m_bullet(nullptr),
	m_image(nullptr)
{}

void Invader::Initialise(const GameContext& gameContext)
{
	// Get cached items.
	m_body = m_entity->GetComponentByTypeFast<Box2DBodyComponent>()->GetBody();
	m_image = m_entity->GetComponentByTypeFast<MoveableQuadComponent>();
	
	// Register for events.
	GameMessageHub::PhysicsInterestRegistration interest;
	interest.m_bodyOfInterest = m_body;
	gameContext.GetMessageHub().SubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &Invader::OnCollide));
	for (auto invIt = m_connectedInvaders.begin(); invIt != m_connectedInvaders.end(); ++invIt)
	{
		gameContext.GetMessageHub().SubscribeGameEvent(m_invaderMap[(*invIt)], Functional::Creator::CreateAction(this, &Invader::OnConnectedInvaderEvent)); 
	}

	m_fireThisFrame = false;
	m_bullet = nullptr;
	m_bulletDead = false;
	m_levelUp = false;
}

void Invader::Cleanup(const GameContext& gameContext)
{
	// Unregister all events.
	GameMessageHub::PhysicsInterestRegistration interest;
	interest.m_bodyOfInterest = m_body;
	gameContext.GetMessageHub().UnsubscribeContactStartEvent(interest, Functional::Creator::CreateAction(this, &Invader::OnCollide));
	for (auto invIt = m_connectedInvaders.begin(); invIt != m_connectedInvaders.end(); ++invIt)
	{
		gameContext.GetMessageHub().UnsubscribeGameEvent(m_invaderMap[(*invIt)], Functional::Creator::CreateAction(this, &Invader::OnConnectedInvaderEvent)); 
	}
	
	// If the bullet is not null, unregister
	UnregisterBullet(gameContext.GetMessageHub());

	// Nullify pointers.
	m_body = nullptr;
	m_image = nullptr;
	
	// Clear data.
	m_connectedInvaders.clear();
	m_invaderMap.clear();
	m_destroyedInvaders.clear();
	m_isPowered = false;
	m_isConnectedToRoot = false;
}

void Invader::CoreUpdate(const GameTime& /*time*/, const GameContext& gameContext)
{
	ClearDestroyedInvaders(gameContext.GetMessageHub());

	if (m_health <= 0)
	{
		m_entity->Destroy();
	}

	if (m_fireThisFrame && m_bullet == nullptr)
	{
		m_bulletDead = false;
		m_bullet = ShouldBeDataDriven::CreateBullet(m_body, gameContext, false);
		gameContext.GetMessageHub().SubscribeGameEvent(m_bullet, Functional::Creator::CreateAction(this, &Invader::OnBulletDiedEvent));
		m_fireThisFrame = false;
	}

	if (m_bulletDead)
	{
		UnregisterBullet(gameContext.GetMessageHub());
	}

	if (m_levelUp)
	{
		LevelUp(gameContext);
	}

	m_image->SetColour(Eigen::Vector4f(1, 1, 1, GetCurrentAlpha()));
}

void Invader::SetupInvader(const Invader::InvaderConfig& config)
{
	m_config = config;
	m_health = config.m_baseHealth;
	if (m_image != nullptr)
	{
		m_image->SetColour(Eigen::Vector4f::Ones());
	}
}

// Adds a connected invader
void Invader::AddConnectedEntity(Invader* invader)
{
	m_invaderMap[invader] = invader->m_entity;
	m_connectedInvaders.push_back(invader);
}

// Gets how many connected invaders there are.
size_t Invader::GetConnectedEntityCount() const
{
	return m_connectedInvaders.size();
}

// Gets the invader at index i.
Invader* Invader::GetConnectedEntity(size_t i) const
{
	return m_connectedInvaders[i];
}

void Invader::OnCollide(const PhysicsContactEvent& contactEvent)
{
	unsigned short category = contactEvent.m_fixtureB->GetFilterData().categoryBits;
	if (m_health > 0 && category == GamePhysicsConstants::c_playerBulletLayer)
	{
		// Get the entity from the target, and get the bullet component.
		ComponentModel::Entity* other = reinterpret_cast<ComponentModel::Entity*>(contactEvent.m_fixtureB->GetBody()->GetUserData());
		Bullet* ob = other->GetComponentByType<Bullet>();

		m_health = (unsigned int)Helpers::Max(0, (int)(m_health - ob->GetDamage()));
		if (m_health == 0)
		{
			m_deathCause = DC_BULLET;
		}
	}
	if (!m_isPowered && (category == GamePhysicsConstants::c_floorLayer || category == GamePhysicsConstants::c_playerTurretLayer))
	{
		m_health = 0;
		m_deathCause = DC_FLOOR;
	}
}

void Invader::OnConnectedInvaderEvent(GameEventTypes::GameEvent event, ComponentModel::Entity* entity)
{
	// We only get events for invaders we care about!
	if (event == GameEventTypes::GE_ENTITY_DESTROYED)
	{
		Invader* otherInvader = entity->GetComponentByTypeFast<Invader>();
		m_destroyedInvaders.push_back(otherInvader);
	}
}

void Invader::ClearDestroyedInvaders(GameMessageHub& hub)
{
	for (auto invIt = m_destroyedInvaders.begin(); invIt != m_destroyedInvaders.end(); ++invIt)
	{
		// Remove subscription...
		Invader* invader = *invIt;
		hub.UnsubscribeGameEvent(m_invaderMap[invader], Functional::Creator::CreateAction(this, &Invader::OnConnectedInvaderEvent)); 
		// Stop caring about them.
		m_connectedInvaders.erase(std::find(m_connectedInvaders.cbegin(), m_connectedInvaders.cend(), invader));
		m_invaderMap.erase(invader);
	}
	m_destroyedInvaders.clear();
}

void Invader::OnBulletDiedEvent(GameEventTypes::GameEvent event, ComponentModel::Entity* entity)
{
	if (event == GameEventTypes::GE_ENTITY_DESTROYED)
	{
		m_bulletDead = true;
		if (entity->GetComponentByTypeFast<Bullet>()->GetHitType() == Bullet::BHT_PLAYER)
		{
			m_levelUp = true;
		}
	}
}

void Invader::UnregisterBullet(GameMessageHub& hub)
{
	hub.UnsubscribeGameEvent(m_bullet, Functional::Creator::CreateAction(this, &Invader::OnBulletDiedEvent));
	m_bulletDead = false;
	m_bullet = nullptr;
}

void Invader::LevelUp(const GameContext& gameContext)
{
	ShouldBeDataDriven::LevelUpInvader(m_entity, this, gameContext);
	m_levelUp = false;
}

float Invader::GetCurrentAlpha()
{
	return ((float)m_health / (float)m_config.m_baseHealth) * (m_isPowered ? 1 : 0.5f);
}