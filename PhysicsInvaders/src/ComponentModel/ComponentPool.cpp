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

#include "ComponentPool.h"
#include "Component.h"
#include "Core/GameTime.h"

namespace ComponentModel
{
	void ComponentPool::DoPhysicsUpdate(const GameTime& time) 
	{ 
		// Just invoke the physics update on all used components
		for (auto cIt = m_usedList.begin(); cIt != m_usedList.end(); ++cIt)
		{ 
			(*cIt)->DoPhysicsUpdate(time, m_gameContext); 
		} 
	}

	void ComponentPool::DoCoreUpdate(const GameTime& time) 
	{ 
		// Do the core update for all used components.
		for (auto cIt = m_usedList.begin(); cIt != m_usedList.end(); ++cIt)
		{ 
			(*cIt)->DoCoreUpdate(time, m_gameContext); 
		}
	}

	void ComponentPool::DoRenderUpdate(const GameTime& time) 
	{ 
		// Do the render update for all used components.
		for (auto cIt = m_usedList.begin(); cIt != m_usedList.end(); ++cIt)
		{ 
			Component* c = (*cIt);
			// NOTE: We're reading a state variable which is written from another thread,
			// however this should be safe, as the value is never set to synchronised 
			// except when only one thread is running.
			if (c->GetState() == Component::CS_SYNCHRONISED || !HasSynchroniseRenderData())
			{
				c->DoRenderUpdate(time, m_gameContext); 
			}
		}
	}

	void ComponentPool::DoSynchroniseRenderData()
	{
		// Clear out the deferred 'release' queue.
		std::for_each(m_releaseSet.begin(), m_releaseSet.end(), [this](Component* c) { c->Cleanup(m_gameContext); m_freeList.push_back(c); });
		m_usedList.remove_if([this](Component* c){return m_releaseSet.find(c) != m_releaseSet.end();});
		m_releaseSet.clear();

		// Cache 2 bools we're going to use a lot in the following update.
		bool hasSynchronise = HasSynchroniseRenderData();
		bool hasNonRender = HasNonRenderUpdate();

		// Do the propagation of enabled state and (if necessary) the synchronise.
		for (auto cIt = m_usedList.begin(); cIt != m_usedList.end(); ++cIt)
		{ 
			Component* c = (*cIt);
			// Always propagate enabled state
			c->PropagateEnabledState();

			// If we've got a synchronise method, and we either don't have a non render method, 
			// or we've received _at least_ one update (if we don't have a non render update we don't
			// need at least one update before calling synch)....
			if (hasSynchronise && (!hasNonRender || c->GetState() == Component::CS_FIRSTUPDATED))
			{
				c->DoSynchroniseRenderData(m_gameContext);
			}
		}

		// Push all freshly acquired components into the used list (ready to go!)
		for (auto acqIt = m_acquireList.begin(); acqIt != m_acquireList.end(); ++acqIt)
		{
			(*acqIt)->DoInitialise(m_gameContext);
			m_usedList.push_back(*acqIt);
		}

		// Clear the acquire list.
		m_acquireList.clear();
	}

	Component* ComponentPool::GetFreeComponent()
	{
		// Get an entry from the free list (note - no resizing here. If there are none available, tough)
		Component* retVal = m_freeList.back();
		m_freeList.pop_back();
		// Push it into the acquire list, ready to go into the used list in the next synch.
		m_acquireList.push_back(retVal);
		retVal->Acquire();
		return retVal;
	}

	void ComponentPool::ReleaseComponentDeferred(Component* c) 
	{
		// Just add the component to the release set, so that it can be returned to the empty pool later on.
		m_releaseSet.insert(c);
	}
};