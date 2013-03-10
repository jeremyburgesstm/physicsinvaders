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

#include "EntityComponentManager.h"

#include "Game/GameContext.h"
#include "Game/Messaging/GameMessageHub.h"
#include "Game/Messaging/GameEventTypes.h"
#include <boost/assert.hpp>

namespace ComponentModel
{
	EntityComponentManager::EntityComponentManager(size_t entityPoolSize)
	{
		m_entities.reserve(entityPoolSize);
		for (size_t i = 0; i < entityPoolSize; ++i)
		{
			Entity* newEnt = new Entity();
			newEnt->SetEntityComponentManager(this);
			m_entities.push_back(newEnt);
			m_freeEntities.push_back(newEnt);
		}
	}

	EntityComponentManager::~EntityComponentManager()
	{
		for (auto enIt = m_entities.begin(); enIt != m_entities.end(); ++enIt)
		{
			delete *enIt;
		}

		m_entities.clear();

		for (auto cpIt = m_componentPools.begin(); cpIt != m_componentPools.end(); ++cpIt)
		{
			delete (*cpIt).second;
		}

		m_componentPools.clear();
	}

	bool SortComponentPools(ComponentPool* first, ComponentPool* second)
	{
		return first->GetUpdatePriority() < second->GetUpdatePriority();
	}

	void EntityComponentManager::RefreshUpdateLists()
	{
		m_coreUpdateList.clear();
		m_physicsUpdateList.clear();
		m_renderUpdateList.clear();
		m_synchroniseList.clear();

		for (auto cpIt = m_componentPools.begin(); cpIt != m_componentPools.end(); ++cpIt)
		{
			ComponentPool* cp = (*cpIt).second;
			if (cp->HasCoreUpdate()) m_coreUpdateList.push_back(cp);
			if (cp->HasPhysicsUpdate()) m_physicsUpdateList.push_back(cp);
			if (cp->HasRenderUpdate()) m_renderUpdateList.push_back(cp);
			
			// Component pool itself does a bunch of work in here, so it needs to cull internally.
			m_synchroniseList.push_back(cp);
		}

		m_coreUpdateList.sort(SortComponentPools);
		m_physicsUpdateList.sort(SortComponentPools);
		m_renderUpdateList.sort(SortComponentPools);
		m_synchroniseList.sort(SortComponentPools);
	}

	Entity* EntityComponentManager::GetFreeEntity()
	{
		Entity* retEnt = m_freeEntities.back();
		m_freeEntities.pop_back();
		retEnt->SetEnabled(true);
		retEnt->SetName("");
		retEnt->SetAlive(true);
		return retEnt;
	}

	void EntityComponentManager::ReleaseEntity(Entity* e)
	{
		BOOST_ASSERT(m_deferredFreeEntities.count(e) == 0);
		m_deferredFreeEntities.insert(e);
		e->SetAlive(false);
		// Notify anyone who's listening that this dude is dead.
		m_gameContext->GetMessageHub().RaiseGameEvent(GameEventTypes::GE_ENTITY_DESTROYED, e);
	}

	Entity* EntityComponentManager::FindEntityByName(const std::string& name)
	{
		// Highly inefficient - we search all entities, including inactive ones (though their names will be
		// unbound). 
		auto entIt = std::find_if(m_entities.begin(), m_entities.end(), [name](Entity* entity){return entity->GetName() == name;});
		return entIt != m_entities.end() ? *entIt : nullptr;
	}

	void EntityComponentManager::ClearAll(const std::set<std::string>* namesToExclude)
	{
		for (auto entIt = m_entities.begin(); entIt != m_entities.end(); ++entIt)
		{
			Entity* entity = (*entIt);
			if (entity->GetAlive())
			{
				if (namesToExclude == nullptr || entity->GetName().empty() || namesToExclude->count(entity->GetName()) == 0)
				{
					entity->Destroy();
				}
			}
		}
		SynchroniseRenderData();
	}

	void EntityComponentManager::DoReleaseEntity(Entity* e)
	{
		e->RemoveAllComponents(this);
		e->SetName("");
		m_freeEntities.push_back(e);
	}

	void EntityComponentManager::PhysicsUpdate(const GameTime& time)
	{
		for (auto cpIt = m_physicsUpdateList.begin(); cpIt != m_physicsUpdateList.end(); ++cpIt)
		{
			(*cpIt)->DoPhysicsUpdate(time);
		}
	}

	void EntityComponentManager::CoreUpdate(const GameTime& time)
	{
		for (auto cpIt = m_coreUpdateList.begin(); cpIt != m_coreUpdateList.end(); ++cpIt)
		{
			(*cpIt)->DoCoreUpdate(time);
		}
	}

	void EntityComponentManager::RenderUpdate(const GameTime& time)
	{
		for (auto cpIt = m_renderUpdateList.begin(); cpIt != m_renderUpdateList.end(); ++cpIt)
		{
			(*cpIt)->DoRenderUpdate(time);
		}
	}

	void EntityComponentManager::SynchroniseRenderData()
	{
		// Clear pending release entities
		for (auto enIt = m_deferredFreeEntities.begin(); enIt != m_deferredFreeEntities.end(); ++enIt)
		{
			DoReleaseEntity(*enIt);
		}
		m_deferredFreeEntities.clear();

		// Let component pools sync.
		for (auto cpIt = m_synchroniseList.begin(); cpIt != m_synchroniseList.end(); ++cpIt)
		{
			(*cpIt)->DoSynchroniseRenderData();
		}
	}
};