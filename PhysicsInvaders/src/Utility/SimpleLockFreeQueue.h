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

/**
 * \class Simple Lock Free Queue
 *
 * Simple queue that stores simple data type by value, and guarantees
 * threadsafety by always staying 1 step behind. Note that this is
 * currently only truly threadsafe on platforms where pointer read/writes
 * are guaranteed to be atomic (order is not, however, important). Only works
 * with exactly one read and one write thread. By making m_last and m_readHead
 * std::atomics, they will work properly once that spec is released.
 *
 * Implementation based directly on the one described by Herb Sutter in
 * "Writing Lock-Free Code: A Corrected Queue" - 
 * http://www.drdobbs.com/high-performance-computing/210604448;jsessionid=AXFEBIM13BVHBQE1GHPCKHWATMY32JVN?pgno=2
 * 
 * @author Jeremy Burgess
 */
template <typename T>
class SimpleLockFreeQueue
{
public:
	SimpleLockFreeQueue(void) :
		m_first(NULL),
		m_readHead(NULL),
		m_last(NULL)
		{
			m_first = new Node( T() );
			m_readHead = m_first;
			m_last = m_first;
		}
	~SimpleLockFreeQueue(void)
	{
		while (m_first)
		{
			Node* toDelete = m_first;
			m_first = m_first->m_next;
			delete toDelete;
		}
		m_last = NULL;
		m_readHead = NULL;
	}

	void Enqueue(const T& newValue)
	{
		// Assign the next pointer.
		m_last->m_next = new Node(newValue);
		m_last = m_last->m_next;

		// Now delete all pending nodes.
		while (m_first != m_readHead)
		{
			Node* toDelete = m_first;
			m_first = toDelete->m_next;
			delete toDelete;
		}
	}

	bool Dequeue(T& storedValue)
	{
		// If it doesn't == last now, it won't later (the enqueue op just advances the
		// last head anyhow). 
		// One point to note is that this means there will always be at least one waiting 
		// entry in the queue
		if (m_readHead != m_last)
		{
			storedValue = m_readHead->m_next->m_value;
			m_readHead = m_readHead->m_next;
			return true;
		}
		return false;
	}

private:
	class Node
	{
	public:
		Node(T value) :
			m_next(NULL),
			m_value(value)
		{}

	public:
		Node* m_next;
		T m_value;
	};

	Node* m_first;
	// These are both "shared variables", should actually be atomic.
	// For now just leave them be.
	volatile Node* m_readHead;
	volatile Node* m_last;
};

