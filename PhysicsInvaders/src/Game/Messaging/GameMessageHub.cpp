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

#include "GameMessageHub.h"
#include "Box2DMessageListener.h"
#include "PhysicsContactEvent.h"

#include <Box2D/Box2D.h>

GameMessageHub::GameMessageHub(b2World* world) :
	m_physicsWorld(world)
{
	m_messageListener = new Box2DMessageListener(this);
	m_physicsWorld->SetContactListener(m_messageListener);
}

GameMessageHub::~GameMessageHub()
{
	m_physicsWorld->SetContactListener(nullptr);
}

void GameMessageHub::RaiseContactStartEvent(b2Contact* contact)
{
	// Just publish directly for now.
	PublishContactStartEvent(contact);
}

void GameMessageHub::RaiseContactEndEvent(b2Contact* contact)
{
	// Just publish directly for now.
	PublishContactEndEvent(contact);
}

void GameMessageHub::RaiseGameEvent(GameEventTypes::GameEvent gameEvent, ComponentModel::Entity* relevantEntity)
{
	// Just publish immediately for now.
	PublishGameEvent(gameEvent, relevantEntity);
}
	
void GameMessageHub::SubscribeContactStartEvent(const GameMessageHub::PhysicsInterestRegistration& interestGroup, Functional::Action<const PhysicsContactEvent&> actionToInvoke)
{
	if (interestGroup.m_bodyOfInterest != nullptr)
	{
		m_contactStartActions.m_bodyMap[interestGroup.m_bodyOfInterest].push_back(actionToInvoke);
	}

	if (interestGroup.m_layerOfInterest != 0)
	{
		m_contactStartActions.m_layerMap[interestGroup.m_layerOfInterest].push_back(actionToInvoke);
	}
}

void GameMessageHub::UnsubscribeContactStartEvent(const GameMessageHub::PhysicsInterestRegistration& interestGroup, Functional::Action<const PhysicsContactEvent&> actionToInvoke)
{
	if (interestGroup.m_bodyOfInterest != nullptr)
	{
		m_contactStartActions.m_bodyMap[interestGroup.m_bodyOfInterest].remove(actionToInvoke);
	}

	if (interestGroup.m_layerOfInterest != 0)
	{
		m_contactStartActions.m_layerMap[interestGroup.m_layerOfInterest].remove(actionToInvoke);
	}
}

void GameMessageHub::SubscribeContactEndEvent(const GameMessageHub::PhysicsInterestRegistration& interestGroup, Functional::Action<const PhysicsContactEvent&> actionToInvoke)
{
	if (interestGroup.m_bodyOfInterest != nullptr)
	{
		m_contactEndActions.m_bodyMap[interestGroup.m_bodyOfInterest].push_back(actionToInvoke);
	}

	if (interestGroup.m_layerOfInterest != 0)
	{
		m_contactEndActions.m_layerMap[interestGroup.m_layerOfInterest].push_back(actionToInvoke);
	}
}

void GameMessageHub::UnsubscribeContactEndEvent(const GameMessageHub::PhysicsInterestRegistration& interestGroup, Functional::Action<const PhysicsContactEvent&> actionToInvoke)
{
	if (interestGroup.m_bodyOfInterest != nullptr)
	{
		m_contactEndActions.m_bodyMap[interestGroup.m_bodyOfInterest].remove(actionToInvoke);
	}

	if (interestGroup.m_layerOfInterest != 0)
	{
		m_contactEndActions.m_layerMap[interestGroup.m_layerOfInterest].remove(actionToInvoke);
	}
}

void GameMessageHub::SubscribeGameEvent(GameEventTypes::GameEvent gameEvent, Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*> action)
{
	m_gameEventListeners[gameEvent].push_back(action);
}

void GameMessageHub::SubscribeGameEvent(ComponentModel::Entity* relevantEntity, Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*> action)
{
	m_entityEventListeners[relevantEntity].push_back(action);
}

void GameMessageHub::UnsubscribeGameEvent(GameEventTypes::GameEvent gameEvent, Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*> action)
{
	m_gameEventListeners[gameEvent].remove(action);
}

void GameMessageHub::UnsubscribeGameEvent(ComponentModel::Entity* relevantEntity, Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*> action)
{
	m_entityEventListeners[relevantEntity].remove(action);
}

void GameMessageHub::PublishContactEvent(b2Contact* contact, GameMessageHub::PhysicsEventActionMap& eventMap)
{
	// Get the fixtures
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	// Create the events
	PhysicsContactEvent evAB(fixtureA, fixtureB);
	PhysicsContactEvent evBA(fixtureB, fixtureA);

	// Notify all subscribers.
	InvokeMappedActions<b2Body*, const PhysicsContactEvent&>(fixtureA->GetBody(), evAB, eventMap.m_bodyMap);
	InvokeMappedActions<b2Body*, const PhysicsContactEvent&>(fixtureB->GetBody(), evBA, eventMap.m_bodyMap);
	InvokeMappedActions<unsigned short, const PhysicsContactEvent&>(fixtureA->GetFilterData().categoryBits, evAB, eventMap.m_layerMap);
	InvokeMappedActions<unsigned short, const PhysicsContactEvent&>(fixtureB->GetFilterData().categoryBits, evBA, eventMap.m_layerMap);
}

void GameMessageHub::PublishContactStartEvent(b2Contact* contact)
{
	PublishContactEvent(contact, m_contactStartActions);
}

void GameMessageHub::PublishContactEndEvent(b2Contact* contact)
{

	PublishContactEvent(contact, m_contactEndActions);
}

void GameMessageHub::PublishGameEvent(GameEventTypes::GameEvent gameEvent, ComponentModel::Entity* relevantEntity)
{
	InvokeMappedActions(gameEvent, gameEvent, relevantEntity, m_gameEventListeners);
	InvokeMappedActions(relevantEntity, gameEvent, relevantEntity, m_entityEventListeners);
}
