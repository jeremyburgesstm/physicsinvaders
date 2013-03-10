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

#include "Invoker.h"

namespace Functional
{
	namespace Internal
	{
		// Declare a type we can use to avoid needing to specify template args for 
		// partial specialisations.
		class None {};
	}
	
	// Forward declare general case.
	template <typename Arg1 = Internal::None, typename Arg2 = Internal::None, typename Arg3 = Internal::None, typename Arg4 = Internal::None>
	class Action;

	/// No arguments action.
	template <>
	class Action<Internal::None, Internal::None, Internal::None, Internal::None>
	{
	public:
		Action(Internal::Invoker0* invoker) : m_invoker(invoker) {}
		Action(const Action& other) : m_invoker(other.m_invoker->Clone()) {}
		~Action() { delete m_invoker; }

		void operator()() {	m_invoker->Invoke(); }
		bool operator==(const Action& other) const { return m_invoker->Equals(other.m_invoker); }
	private:
		Internal::Invoker0* m_invoker;
	};

	/// 1 Arg action
	template <typename Arg1>
	class Action<Arg1, Internal::None, Internal::None, Internal::None>
	{
	public:
		Action(Internal::Invoker1<Arg1>* invoker) : m_invoker(invoker) {}
		Action(const Action& other) : m_invoker(other.m_invoker->Clone()) {}
		~Action() { delete m_invoker; }

		void operator()(Arg1 arg1) { m_invoker->Invoke(arg1); }
		bool operator==(const Action<Arg1>& other) const { return m_invoker->Equals(other.m_invoker); }
	private:
		Internal::Invoker1<Arg1>* m_invoker;
	};

	/// 2 Args action.
	template <typename Arg1, typename Arg2>
	class Action<Arg1, Arg2, Internal::None, Internal::None>
	{
	public:
		Action(Internal::Invoker2<Arg1, Arg2>* invoker) : m_invoker(invoker) {}
		Action(const Action& other) : m_invoker(other.m_invoker->Clone()) {}
		~Action() { delete m_invoker; }

		void operator()(Arg1 arg1, Arg2 arg2) { m_invoker->Invoke(arg1, arg2); }
		bool operator==(const Action<Arg1, Arg2>& other) const { return m_invoker->Equals(other.m_invoker); }
	private:
		Internal::Invoker2<Arg1, Arg2>* m_invoker;
	};

	/// 3 Args action.
	template <typename Arg1, typename Arg2, typename Arg3>
	class Action<Arg1, Arg2, Arg3, Internal::None>
	{
	public:
		Action(Internal::Invoker3<Arg1, Arg2, Arg3>* invoker) : m_invoker(invoker) {}
		Action(const Action& other) : m_invoker(other.m_invoker->Clone()) {}
		~Action() { delete m_invoker; }

		void operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3) { m_invoker->Invoke(arg1, arg2, arg3); }
		bool operator==(const Action<Arg1, Arg2, Arg3>& other) const { return m_invoker->Equals(other.m_invoker); }
	private:
		Internal::Invoker3<Arg1, Arg2, Arg3>* m_invoker;
	};
	
	/// 4 Args action.
	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	class Action
	{
	public:
		Action(Internal::Invoker4<Arg1, Arg2, Arg3, Arg4>* invoker) : m_invoker(invoker) {}
		Action(const Action& other) : m_invoker(other.m_invoker->Clone()) {}
		~Action() { delete m_invoker; }

		void operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { m_invoker->Invoke(arg1, arg2, arg3, arg4); }
		bool operator==(const Action<Arg1, Arg2, Arg3, Arg4>& other) const 
		{ 
			return m_invoker->Equals(other.m_invoker); 
		}
	private:
		Internal::Invoker4<Arg1, Arg2, Arg3, Arg4>* m_invoker;
	};

	/// Class needed to nest action create methods in, as otherwise they will show up as multiply defined.
	class Creator
	{
	public:
		static Action<> CreateAction(void(*DoFuncAction)(void))
		{
			return Action<Internal::None, Internal::None>(new Internal::FunctionInvoker0(DoFuncAction));
		};

		template <class Caller>
		static Action<> CreateAction(Caller* caller, void(Caller::*DoMethodAction)(void))
		{
			return Action<Internal::None, Internal::None>(new Internal::MethodInvoker0<Caller>(caller, DoMethodAction));
		};

		template <typename Arg1>
		static Action<Arg1> CreateAction(void(*DoFuncAction)(Arg1))
		{
			return Action<Arg1, Internal::None>(new Internal::FunctionInvoker1<Arg1>(DoFuncAction));
		};

		template <class Caller, typename Arg1>
		static Action<Arg1> CreateAction(Caller* caller, void(Caller::*DoMethodAction)(Arg1))
		{
			return Action<Arg1, Internal::None>(new Internal::MethodInvoker1<Caller, Arg1>(caller, DoMethodAction));
		};

		template <typename Arg1, typename Arg2>
		static Action<Arg1, Arg2> CreateAction(void(*DoFuncAction)(Arg1, Arg2))
		{
			return Action<Arg1, Arg2>(new Internal::FunctionInvoker2<Arg1, Arg2>(DoFuncAction));
		};

		template <class Caller, typename Arg1, typename Arg2>
		static Action<Arg1, Arg2> CreateAction(Caller* caller, void(Caller::*DoMethodAction)(Arg1, Arg2))
		{
			return Action<Arg1, Arg2>(new Internal::MethodInvoker2<Caller, Arg1, Arg2>(caller, DoMethodAction));
		};

		template <typename Arg1, typename Arg2, typename Arg3>
		static Action<Arg1, Arg2, Arg3> CreateAction(void(*DoFuncAction)(Arg1, Arg2, Arg3))
		{
			return Action<Arg1, Arg2, Arg3>(new Internal::FunctionInvoker3<Arg1, Arg2, Arg3>(DoFuncAction));
		};

		template <class Caller, typename Arg1, typename Arg2, typename Arg3>
		static Action<Arg1, Arg2, Arg3> CreateAction(Caller* caller, void(Caller::*DoMethodAction)(Arg1, Arg2, Arg3))
		{
			return Action<Arg1, Arg2, Arg3>(new Internal::MethodInvoker3<Caller, Arg1, Arg2, Arg3>(caller, DoMethodAction));
		};
	
		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		static Action<Arg1, Arg2, Arg3, Arg4> CreateAction(void(*DoFuncAction)(Arg1, Arg2, Arg3, Arg4))
		{
			return Action<Arg1, Arg2, Arg3, Arg4>(new Internal::FunctionInvoker4<Arg1, Arg2, Arg3, Arg4>(DoFuncAction));
		};

		template <class Caller, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		static Action<Arg1, Arg2, Arg3, Arg4> CreateAction(Caller* caller, void(Caller::*DoMethodAction)(Arg1, Arg2, Arg3, Arg4))
		{
			return Action<Arg1, Arg2, Arg3, Arg4>(new Internal::MethodInvoker4<Caller, Arg1, Arg2, Arg3, Arg4>(caller, DoMethodAction));
		};
	};
};