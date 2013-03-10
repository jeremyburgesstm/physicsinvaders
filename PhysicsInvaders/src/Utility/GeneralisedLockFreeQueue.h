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
 * \class Generalised Lock Free Queue
 *
 * "Lock free" queue that is threadsafe both for multiple consumers and 
 * producers. Not truly lock free, however there should never be contention
 * between consumption and production. Consumers and producers however
 * block other entities of their own type.
 * Stores a pointer type.
 *
 * Implementation based directly on the one described by Herb Sutter in
 * "Writing Lock-Free Code: A Corrected Queue" - 
 * http://www.drdobbs.com/high-performance-computing/210604448;jsessionid=AXFEBIM13BVHBQE1GHPCKHWATMY32JVN?pgno=2
 * 
 * @author Jeremy Burgess
 */
template<class T>
class GeneralisedLockFreeQueue
{
public:
	GeneralisedLockFreeQueue(void)
	{
		m_first = m_last = new Node(nullptr);
		m_enqueueLock = 0;
		m_dequeueLock = 0;
	}
	~GeneralisedLockFreeQueue(void)
	{
		while (m_first)
		{
			Node* toDelete = m_first;
			m_first = m_first->m_next;
			delete toDelete;
		}
	}

	void Enqueue(T* value)
	{
		// Construct our new node
		Node* newNode = new Node(value);
		
		// Acquire exclusive access
		// TODO: Wrap the atomic write in something that can be reimplemented per platform.
		while (InterlockedCompareExchange(&m_enqueueLock, 1, 0) != 0) {}

		// Now update the last pointer.
		m_last->m_next = newNode;
		m_last = m_last->m_next;

		// Drop exclusive access
		InterlockedExchange(&m_enqueueLock, 0);
	}

	T* Dequeue()
	{
		// Set the returned value
		T* returnedValue = nullptr;

		// Acquire exclusive access
		// TODO: Wrap the atomic write in something that can be reimplemented per platform.
		while (InterlockedCompareExchange(&m_dequeueLock, 1, 0) != 0) {}

		// Now, check to see if the first pointer has a next pointer.
		Node* first = m_first;
		Node* next = m_first->m_next;
		if (next != nullptr)
		{
			returnedValue = next->m_value;
			// Now manipulate the list - after this other things can do whatever they like.
			m_first = next;
			m_first->m_value = nullptr;
			
			// Drop exclusive access
			InterlockedExchange(&m_dequeueLock, 0);

			// Cleanup the data and move on.
			// Note - in Sutter's implementation he actually allocates new things in the 
			// list. Because we're assuming the list just holds pointers (i.e., it doesn't
			// allocate anything other than nodes), the only threadsafe action at this point is deleting
			// first.
			delete first;
		}
		else
		{
			// Drop exclusive access
			InterlockedExchange(&m_dequeueLock, 0);
		}

		// Return something useful.
		return returnedValue;
	}

private:
	class Node
	{
	public:
		Node(T* newValue) :
			m_value(newValue),
			m_next(nullptr) {}
		T* m_value;
		Node* m_next;
	};

private:
	Node* m_first;
	Node* m_last;
	
	volatile unsigned int m_enqueueLock;
	volatile unsigned int m_dequeueLock;
};

