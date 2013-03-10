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

#include "Entity.h"

class GameTime;
class GameContext;

namespace ComponentModel
{
	/**
	 * \class Component
	 *
	 * Base class for all components. Stores the current component state & provides
	 * overrideable methods for all parts of component behaviour.
	 */
	class Component
	{
	public: 
		enum ComponentState
		{
			CS_INACTIVE,
			CS_INITIALISED,
			CS_FIRSTUPDATED,
			CS_SYNCHRONISED,
		};
		friend class Entity; // Only entity can set itself as the entity for a component.
		friend class ComponentPool;

	public:
		Component() :
			m_entity(nullptr),
			m_state(CS_INACTIVE)
		{ }
		virtual ~Component() {}

		void DoInitialise(const GameContext& gameContext);
		void DoCoreUpdate(const GameTime& time, const GameContext& /*gameContext*/);
		void DoPhysicsUpdate(const GameTime& time, const GameContext& /*gameContext*/);
		void DoSynchroniseRenderData(const GameContext& /*gameContext*/);
		void DoRenderUpdate(const GameTime& time, const GameContext& /*gameContext*/);

		virtual void Initialise(const GameContext& /*gameContext*/) {}
		virtual void Cleanup(const GameContext& /*gameContext*/) {}
		virtual void PhysicsUpdate(const GameTime& /*time*/, const GameContext& /*gameContext*/) {}
		virtual void FirstCoreUpdate(const GameTime&, const GameContext& /*gameContext*/) {}
		virtual void CoreUpdate(const GameTime& /*time*/, const GameContext& /*gameContext*/) {}
		virtual void RenderUpdate(const GameTime& /*time*/, const GameContext& /*gameContext*/) {}
		virtual void SynchroniseRenderData(const GameContext& /*gameContext*/) {}

		ComponentState GetState() const { return m_state; }
		
		inline void SetEnabled(bool enabled) { m_enabled = enabled; }
		inline bool GetEnabled() const { return m_enabled; }

	private:
		void SetEntity(Entity* entity)
		{
			m_entity = entity;
		}

		/// Invoked as soon as the component is 'given out',
		/// Initialises any values which should be correct before
		/// anyone even looks at the component.
		void Acquire();

		/// Invoked by component pool during updates.
		void PropagateEnabledState();
	protected:
		Entity* m_entity;
		bool m_enabled;

	private:
		ComponentState m_state;
		bool m_renderEnabled;
	};
};