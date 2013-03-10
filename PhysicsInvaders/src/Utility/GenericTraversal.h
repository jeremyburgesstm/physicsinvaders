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
 * Generic implementation classes of traversals.
 */
namespace Traversal
{
	template<class Type, class Functor>
	void DepthFirstAllChildren(Type* root, 
		size_t(Type::*GET_CHILD_COUNT)(void) const,
		Type*(Type::*GET_CHILD)(size_t) const,
		const Functor& functor)
	{
		functor(root);

		for (size_t i = 0; i < (root->*GET_CHILD_COUNT)(); ++i)
		{
			Type* val = (root->*GET_CHILD)(i);
			DepthFirstAllChildren(
				val,
				GET_CHILD_COUNT,
				GET_CHILD,
				functor);
		}
	}

	template<class Type, class Predicate, class Functor>
	void DepthFirstPreRecurse(Type* root, 
		size_t(Type::*GET_CHILD_COUNT)(void) const,
		Type*(Type::*GET_CHILD)(size_t) const,
		const Predicate& predicate,
		const Functor& functor)
	{
		if (!predicate(root))
		{
			return;
		}

		functor(root);

		for (size_t i = 0; i < (root->*GET_CHILD_COUNT)(); ++i)
		{
			Type* val = (root->*GET_CHILD)(i);
			DepthFirstPreRecurse(
				val,
				GET_CHILD_COUNT,
				GET_CHILD,
				predicate,
				functor);
		}
	}
};