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

class GameContext;

// STL
#include <map>
#include <vector>
#include <string>
#include <list>
#include <set>

// Boost
#include <boost/noncopyable.hpp>

// Component model
#include "ComponentPool.h"
#include "Entity.h"
#include "Component.h"

namespace ComponentModel
{
	/** 
	 * \Class EntityComponentManager
	 * 
	 * Heart of the entity component model, this class holds all the entities and all the component pools.
	 *
	 * It drives all the updates, and provides the interface for releasing entities once done with them.
	 */
	class EntityComponentManager : public boost::noncopyable
	{
	public:
		EntityComponentManager(size_t entityPoolSize);
		~EntityComponentManager();

		void SetGameContext(const GameContext* context) { m_gameContext = context; }

		/// Adds a component type which can be used, pre-allocs the pool to the 
		/// specified size, and sets it to update with the specified priorty (lower = earlier)
		template <class T> void AddComponentType(size_t poolSize, int updatePriority)
		{
			m_componentPools[typeid(T).hash_code()] = new TypedComponentPool<T>(poolSize, updatePriority, *m_gameContext);
		}

		/// Adds a component of type to the entity in question and returns you a pointer to the 
		/// newly acquired component. Note that the component will not have had 'initialise' 
		/// called, but you can setup data on it at this stage.
		template <class T> T* AddComponent(Entity* entity)
		{
			// Get the component - no need to do a dynamic cast, as the lookup should handle this.
			// Note that this could cause badness if the "T" does not exist - good thing I'm the 
			// only one writing code for this!
			T* component = (T*)m_componentPools[typeid(T).hash_code()]->GetFreeComponent();
			entity->AddComponent(component);
			return component;
		}

		/// Removes a component from the entity in question and enqueues it so that in the next synchronise it will be
		/// removed from the active list.
		template <class T> void RemoveComponent(Entity* entity)
		{
			T* component = entity->GetComponentByType<T>();
			entity->RemoveComponent(component);
			m_componentPools[typeid(T).hash_code()]->ReleaseComponentDeferred(component);
		}
		
		/// Removes a component by Component pointer
		void RemoveComponent(Entity* entity, Component* component)
		{
			entity->RemoveComponent(component);	
			const type_info& ti = typeid(*component);
			m_componentPools[ti.hash_code()]->ReleaseComponentDeferred(component);
		}

		/// Gets a free entity
		Entity* GetFreeEntity();

		/// Releases an entity (also releasing all of its components)
		void ReleaseEntity(Entity* entity);

		/// Finds the first entity which has the name provided, or nullptr if none exist
		/// This method is slow - requires a search through _all_ entities! Use it sparingly.
		Entity* FindEntityByName(const std::string& name);

		/// After all pools have been created, invoke this to setup the update lists.
		void RefreshUpdateLists();

		/// Scorched earth "Clear all" destroys all existing entities
		/// excepting any with a name in the list specified and forces one synchronise in order
		/// to ensure they're dead.
		///
		/// Note that the exclusion criteria is undesirable, however some mechanism is required to
		/// allow objects we want to persist to survive, and for now this is the path of least resistance
		/// that still ensures the most usefulness.
		void ClearAll(const std::set<std::string>* namesToExclude);

		/// \name All update methods
		/// @{
			void PhysicsUpdate(const GameTime& /*time*/);
			void CoreUpdate(const GameTime& /*time*/);
			void RenderUpdate(const GameTime& /*time*/);
			void SynchroniseRenderData();
		/// @}
	private:
		/// Actually does the release on an entity.
		void DoReleaseEntity(Entity* entity);

		// Reference to the game context (we hold this)
		const GameContext* m_gameContext;

		// Component pool management
		std::map<size_t,  ComponentPool*> m_componentPools;
		std::list<ComponentPool*> m_physicsUpdateList;
		std::list<ComponentPool*> m_coreUpdateList;
		std::list<ComponentPool*> m_renderUpdateList;
		std::list<ComponentPool*> m_synchroniseList;

		// Entity pool.
		std::vector<Entity*> m_entities;
		std::list<Entity*> m_freeEntities;
		std::set<Entity*> m_deferredFreeEntities;
	};
};