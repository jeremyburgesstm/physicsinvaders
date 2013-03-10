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

#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <boost/noncopyable.hpp>

class GameTime;
class GameContext;

namespace ComponentModel
{
	class Component;

	/**
	 * \class ComponentPool
	 *
	 * Base class for component pools. Manages actually doing the updates for a given
	 * type of component.
	 *
	 * Note that the priority & size of a given pool are set at creation time.
	 */
	class ComponentPool : public boost::noncopyable
	{
	public:
		ComponentPool(size_t poolSize, int updatePriority, const GameContext& gameContext) :
			m_updatePriority(updatePriority),
			m_gameContext(gameContext)
		{ 
			m_components.reserve(poolSize);
		}

		virtual bool HasPhysicsUpdate() const = 0;
		virtual bool HasCoreUpdate() const = 0;
		bool HasNonRenderUpdate() const { return HasPhysicsUpdate() || HasCoreUpdate(); }
		virtual bool HasRenderUpdate() const = 0;
		virtual bool HasSynchroniseRenderData() const = 0;

		void DoPhysicsUpdate(const GameTime& time);
		void DoCoreUpdate(const GameTime& time);
		void DoRenderUpdate(const GameTime& time);
		void DoSynchroniseRenderData();

		int GetUpdatePriority() const { return m_updatePriority; }

		Component* GetFreeComponent();
		void ReleaseComponentDeferred(Component* c);

	protected:
		std::vector<Component*> m_components;
		std::list<Component*> m_freeList;
		std::list<Component*> m_usedList;
		std::list<Component*> m_acquireList;
		std::set<Component*> m_releaseSet;

		int m_updatePriority;

		const GameContext& m_gameContext;
 	};

	/**
	 * Typed component provides type specific functionality for component pools +
	 * instantiation of all components of that type.
	 */
	template <class T>
	class TypedComponentPool : public ComponentPool
	{
	public:
		TypedComponentPool(size_t poolSize, int updatePriority, const GameContext& gameContext) :
			ComponentPool(poolSize, updatePriority, gameContext)
		{
			for (size_t i = 0; i < poolSize; ++i)
			{
				T* newComponent = new T();
				m_components.push_back(newComponent);
				m_freeList.push_back(newComponent);
			}
		}
		~TypedComponentPool()
		{
			m_freeList.clear();
			m_usedList.clear();
			std::for_each(m_components.begin(), m_components.end(), [](Component* c) { delete c; });
			m_components.clear();
		}

		virtual bool HasPhysicsUpdate() const { return T::HasPhysicsUpdate(); }
		virtual bool HasCoreUpdate() const { return T::HasCoreUpdate(); }
		virtual bool HasRenderUpdate() const { return T::HasRenderUpdate(); }
		virtual bool HasSynchroniseRenderData() const { return T::HasSynchroniseRenderData(); }
	};
};