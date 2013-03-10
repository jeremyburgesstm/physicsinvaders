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

namespace Functional
{
	namespace Internal
	{
		enum InvokerType
		{
			IT_FUNCTION,
			IT_METHOD
		};

		class Invoker0
		{
		public:
			virtual void Invoke() = 0;
			virtual bool Equals(Invoker0* other) const = 0;
			virtual InvokerType GetInvokerType() const = 0;
			virtual Invoker0* Clone() const = 0;
		};

		template <typename Arg1>
		class Invoker1
		{
		public:
			virtual void Invoke(Arg1 arg1) = 0;
			virtual bool Equals(Invoker1* other) const = 0;
			virtual InvokerType GetInvokerType() const = 0;
			virtual Invoker1* Clone() const = 0;
		};
		
		template <typename Arg1, typename Arg2>
		class Invoker2
		{
		public:
			virtual void Invoke(Arg1 arg1, Arg2 arg2) = 0;
			virtual bool Equals(Invoker2* other) const = 0;
			virtual InvokerType GetInvokerType() const = 0;
			virtual Invoker2* Clone() const = 0;
		};

		template <typename Arg1, typename Arg2, typename Arg3>
		class Invoker3
		{
		public:
			virtual void Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3) = 0;
			virtual bool Equals(Invoker3* other) const = 0;
			virtual InvokerType GetInvokerType() const = 0;
			virtual Invoker3* Clone() const = 0;
		};

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		class Invoker4
		{
		public:
			virtual void Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) = 0;
			virtual bool Equals(Invoker4* other) const = 0;
			virtual InvokerType GetInvokerType() const = 0;
			virtual Invoker4* Clone() const = 0;
		};
		
		template <class Caller>
		class MethodInvoker0 : public Invoker0
		{
		public:
			MethodInvoker0(Caller* caller, void(Caller::*Invoke)(void)) : m_caller(caller), m_invoke(Invoke) {}

			virtual void Invoke() {	(m_caller->*m_invoke)(); }
			virtual bool Equals(Invoker0* other) const 
			{
				if (other->GetInvokerType() == IT_METHOD)
				{
					MethodInvoker0* otherTyped = static_cast<MethodInvoker0*>(other);
					return (otherTyped->m_caller == m_caller && otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_METHOD; }
			virtual Invoker0* Clone() const { return new MethodInvoker0(m_caller, m_invoke); }
		private:
			Caller* m_caller;
			void(Caller::*m_invoke)(void);
		};

		template <class Caller, typename Arg1>
		class MethodInvoker1 : public Invoker1<Arg1>
		{
		public:
			MethodInvoker1(Caller* caller, void(Caller::*Invoke)(Arg1)) : m_caller(caller), m_invoke(Invoke) {}

			virtual void Invoke(Arg1 arg1) { (m_caller->*m_invoke)(arg1); }
			virtual bool Equals(Invoker1* other) const 
			{
				if (other->GetInvokerType() == IT_METHOD)
				{
					MethodInvoker1* otherTyped = static_cast<MethodInvoker1*>(other);
					return (otherTyped->m_caller == m_caller && otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_METHOD; }
			virtual Invoker1* Clone() const { return new MethodInvoker1(m_caller, m_invoke); }
		private:
			Caller* m_caller;
			void(Caller::*m_invoke)(Arg1);
		};

		template <class Caller, typename Arg1, typename Arg2>
		class MethodInvoker2 : public Invoker2<Arg1, Arg2>
		{
		public:
			MethodInvoker2(Caller* caller, void(Caller::*Invoke)(Arg1, Arg2)) : m_caller(caller), m_invoke(Invoke) {}

			virtual void Invoke(Arg1 arg1, Arg2 arg2) { (m_caller->*m_invoke)(arg1, arg2); }
			virtual bool Equals(Invoker2* other) const 
			{
				if (other->GetInvokerType() == IT_METHOD)
				{
					MethodInvoker2* otherTyped = static_cast<MethodInvoker2*>(other);
					return (otherTyped->m_caller == m_caller && otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_METHOD; }
			virtual Invoker2* Clone() const { return new MethodInvoker2(m_caller, m_invoke); }
		private:
			Caller* m_caller;
			void(Caller::*m_invoke)(Arg1, Arg2);
		};

		template <class Caller, typename Arg1, typename Arg2, typename Arg3>
		class MethodInvoker3 : public Invoker3<Arg1, Arg2, Arg3>
		{
		public:
			MethodInvoker3(Caller* caller, void(Caller::*Invoke)(Arg1, Arg2, Arg3)) : m_caller(caller), m_invoke(Invoke) {}

			virtual void Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3) { (m_caller->*m_invoke)(arg1, arg2, arg3); }
			virtual bool Equals(Invoker3* other) const 
			{
				if (other->GetInvokerType() == IT_METHOD)
				{
					MethodInvoker3* otherTyped = static_cast<MethodInvoker3*>(other);
					return (otherTyped->m_caller == m_caller && otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_METHOD; }
			virtual Invoker3* Clone() const { return new MethodInvoker3(m_caller, m_invoke); }
		private:
			Caller* m_caller;
			void(Caller::*m_invoke)(Arg1, Arg2, Arg3);
		};

		template <class Caller, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		class MethodInvoker4 : public Invoker4<Arg1, Arg2, Arg3, Arg4>
		{
		public:
			MethodInvoker4(Caller* caller, void(Caller::*Invoke)(Arg1, Arg2, Arg3, Arg4)) : m_caller(caller), m_invoke(Invoke) {}

			virtual void Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { (m_caller->*m_invoke)(arg1, arg2, arg3, arg4); }
			virtual bool Equals(Invoker4* other) const 
			{
				if (other->GetInvokerType() == IT_METHOD)
				{
					MethodInvoker4* otherTyped = static_cast<MethodInvoker4*>(other);
					return (otherTyped->m_caller == m_caller && otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_METHOD; }
			virtual Invoker4* Clone() const { return new MethodInvoker4(m_caller, m_invoke); }
		private:
			Caller* m_caller;
			void(Caller::*m_invoke)(Arg1, Arg2, Arg3, Arg4);
		};

		class FunctionInvoker0 : public Invoker0
		{
		public:
			FunctionInvoker0(void(*Invoke)()) :	m_invoke(Invoke) { }

			virtual void Invoke() {	m_invoke();	}
			virtual bool Equals(Invoker0* other) const 
			{
				if (other->GetInvokerType() == IT_FUNCTION)
				{
					FunctionInvoker0* otherTyped = static_cast<FunctionInvoker0*>(other);
					return (otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_FUNCTION; }
			virtual Invoker0* Clone() const { return new FunctionInvoker0(m_invoke); }
		private:
			void(*m_invoke)(void);
		};

		template <typename Arg1>
		class FunctionInvoker1 : public Invoker1<Arg1>
		{
		public:
			FunctionInvoker1(void(*Invoke)(Arg1)) : m_invoke(Invoke) { }

			virtual void Invoke(Arg1 arg1) { m_invoke(arg1); }
			virtual bool Equals(Invoker1* other) const 
			{
				if (other->GetInvokerType() == IT_FUNCTION)
				{
					FunctionInvoker1* otherTyped = static_cast<FunctionInvoker1*>(other);
					return (otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_FUNCTION; }
			virtual Invoker1* Clone() const { return new FunctionInvoker1(m_invoke); }
		private:
			void(*m_invoke)(Arg1);
		};

		template <typename Arg1, typename Arg2>
		class FunctionInvoker2 : public Invoker2<Arg1, Arg2>
		{
		public:
			FunctionInvoker2(void(*Invoke)(Arg1, Arg2)) : m_invoke(Invoke) { }

			virtual void Invoke(Arg1 arg1, Arg2 arg2) { m_invoke(arg1, arg2); }
			virtual bool Equals(Invoker2* other) const 
			{
				if (other->GetInvokerType() == IT_FUNCTION)
				{
					FunctionInvoker2* otherTyped = static_cast<FunctionInvoker2*>(other);
					return (otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_FUNCTION; }
			virtual Invoker2* Clone() const { return new FunctionInvoker2(m_invoke); }
		private:
			void(*m_invoke)(Arg1, Arg2);
		};

		template <typename Arg1, typename Arg2, typename Arg3>
		class FunctionInvoker3 : public Invoker3<Arg1, Arg2, Arg3>
		{
		public:
			FunctionInvoker3(void(*Invoke)(Arg1, Arg2, Arg3)) : m_invoke(Invoke) { }

			virtual void Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3) { m_invoke(arg1, arg2, arg3); }
			virtual bool Equals(Invoker3* other) const 
			{
				if (other->GetInvokerType() == IT_FUNCTION)
				{
					FunctionInvoker3* otherTyped = static_cast<FunctionInvoker3*>(other);
					return (otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_FUNCTION; }
			virtual Invoker3* Clone() const { return new FunctionInvoker3(m_invoke); }
		private:
			void(*m_invoke)(Arg1, Arg2, Arg3);
		};

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		class FunctionInvoker4 : public Invoker4<Arg1, Arg2, Arg3, Arg4>
		{
		public:
			FunctionInvoker4(void(*Invoke)(Arg1, Arg2, Arg3, Arg4)) : m_invoke(Invoke) { }

			virtual void Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { m_invoke(arg1, arg2, arg3, arg4); }
			virtual bool Equals(Invoker4* other) const 
			{
				if (other->GetInvokerType() == IT_FUNCTION)
				{
					FunctionInvoker4* otherTyped = static_cast<FunctionInvoker4*>(other);
					return (otherTyped->m_invoke == m_invoke);
				}
				return false;
			}
			virtual InvokerType GetInvokerType() const { return IT_FUNCTION; }
			virtual Invoker4* Clone() const { return new FunctionInvoker4(m_invoke); }
		private:
			void(*m_invoke)(Arg1, Arg2, Arg3, Arg4);
		};
	};
};
