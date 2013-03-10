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

// Forward declarations.
class b2Contact;
class b2Body;
class b2Fixture;
class b2World;
class Box2DMessageListener;
struct PhysicsContactEvent;
namespace ComponentModel
{
	class Entity;
};

#include "Core/Functional/Action.h"
#include "GameEventTypes.h"
#include <map>
#include <list>

/**
 * Class which acts as a hub for messaging. Supports publish/subscribe 
 * and understands what to do with different message types. Will publish 
 * all incoming messages as soon as they arrive (for now). This means that
 * in the current implementation it is expected that no messages will be
 * generated outside of the main thread (if this becomes the case the
 * message hub will need to cache these in a queue and push them out 
 * during a 'pump' phase).
 *
 * In case this is needed there are 4 methods associated with a given event:
 *  - RaiseEvent (Tells the hub the event has happened)
 *  - PublishEvent (Private, the hub uses this to notify all subscribers)
 *  - SubscribeEvent (Tells the hub to notify a given listener action, with specific interest actions)
 *  - UnsubscribeEvent (Tells the hub to no longer notify a given listener action)
 * NOTE: Subscribe/Unsubscribes should not be called during a publish.
 */
class GameMessageHub
{
public:
	/**
	 * Structure describing a registration of interest for a 
	 * physics event.
	 */
	struct PhysicsInterestRegistration
	{
		PhysicsInterestRegistration() :
			m_layerOfInterest(0),
			m_bodyOfInterest(nullptr)
		{ }

		/// Layer of interest for the registration. Note that this cannot be
		/// a true bitmask - the system internally will only use this to lookup
		/// single layers, and will only work for entities which are in only
		/// a single category.
		unsigned short m_layerOfInterest;

		/// A body you want to listen to.
		b2Body* m_bodyOfInterest;
	};

public:
	GameMessageHub(b2World* world);
	~GameMessageHub();

	void RaiseContactStartEvent(b2Contact* /*contact*/);
	void RaiseContactEndEvent(b2Contact* /*contact*/);
	void RaiseGameEvent(GameEventTypes::GameEvent gameEvent, ComponentModel::Entity* relevantEntity);
	
	void SubscribeContactStartEvent(const PhysicsInterestRegistration& /*interestGroup*/, Functional::Action<const PhysicsContactEvent&> /*actionToInvoke*/);
	void UnsubscribeContactStartEvent(const PhysicsInterestRegistration& /*interestGroup*/, Functional::Action<const PhysicsContactEvent&> /*actionToInvoke*/);

	void SubscribeContactEndEvent(const PhysicsInterestRegistration& /*interestGroup*/, Functional::Action<const PhysicsContactEvent&> /*actionToInvoke*/);
	void UnsubscribeContactEndEvent(const PhysicsInterestRegistration& /*interestGroup*/, Functional::Action<const PhysicsContactEvent&> /*actionToInvoke*/);

	void SubscribeGameEvent(GameEventTypes::GameEvent gameEvent, Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*>);
	void SubscribeGameEvent(ComponentModel::Entity* relevantEntity, Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*>);
	void UnsubscribeGameEvent(GameEventTypes::GameEvent gameEvent, Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*>);
	void UnsubscribeGameEvent(ComponentModel::Entity* relevantEntity, Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*>);

private:
	struct PhysicsEventActionMap
	{
		std::map< b2Body*, std::list< Functional::Action<const PhysicsContactEvent&> > > m_bodyMap;
		std::map< unsigned short, std::list< Functional::Action<const PhysicsContactEvent&> > > m_layerMap;
	};

private:
	void PublishContactStartEvent(b2Contact* /*contact*/);
	void PublishContactEndEvent(b2Contact* /*contact*/);
	void PublishContactEvent(b2Contact* contact, PhysicsEventActionMap& eventMap);
	void PublishGameEvent(GameEventTypes::GameEvent gameEvent, ComponentModel::Entity* relevantEntity);

	/// Helper method to invoke a bunch of actions - just avoids writing unnecessary boilerplate.
	template<typename Key, typename Event>
	void InvokeMappedActions(Key key, Event e, std::map< Key, std::list< Functional::Action<Event> > > map)
	{
		if (map.count(key) > 0)
		{
			for (auto actIt = map[key].begin(); actIt != map[key].end(); ++actIt)
			{
				(*actIt)(e);
			}
		}
	}

	/// Helper method to invoke a bunch of actions - just avoids writing unnecessary boilerplate.
	template<typename Key, typename EventArg1, typename EventArg2>
	void InvokeMappedActions(Key key, EventArg1 e1, EventArg2 e2, std::map< Key, std::list< Functional::Action<EventArg1, EventArg2> > > map)
	{
		if (map.count(key) > 0)
		{
			for (auto actIt = map[key].begin(); actIt != map[key].end(); ++actIt)
			{
				(*actIt)(e1, e2);
			}
		}
	}

private:
	b2World* m_physicsWorld;
	Box2DMessageListener* m_messageListener;
	PhysicsEventActionMap m_contactStartActions;
	PhysicsEventActionMap m_contactEndActions;
	std::map<GameEventTypes::GameEvent, std::list< Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*> > > m_gameEventListeners;
	std::map<ComponentModel::Entity*, std::list< Functional::Action<GameEventTypes::GameEvent, ComponentModel::Entity*> > > m_entityEventListeners;
};