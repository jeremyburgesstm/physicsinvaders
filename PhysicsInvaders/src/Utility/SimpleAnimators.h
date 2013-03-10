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

#define _USE_MATH_DEFINES
#include <math.h>
#include "Helpers.h"

namespace SimpleAnimators
{
	class ISimpleAnimator
	{
	public:
		virtual void UpdateValue(float delta) = 0;
	};

	template<class T>
	class SinAnimator : public ISimpleAnimator
	{
	public:
		SinAnimator(T* target, void(T::*SetFloatValue)(float), 
			float period, float amplitude, float offset, float minClamp, float maxClamp) :
			m_target(target),
			m_SetFloatValue(SetFloatValue),
			m_period(period),
			m_amplitude(amplitude),
			m_offset(offset),
			m_minClamp(minClamp),
			m_maxClamp(maxClamp),
			m_time(0)
		{ }

		virtual void UpdateValue(float delta)
		{
			float value = Helpers::Clamp(sin(((float)M_PI * 2 * m_time) / m_period) * m_amplitude + m_offset, m_minClamp, m_maxClamp);
			(m_target->*m_SetFloatValue)(value);
			m_time += delta;
		}
	private:
		T* m_target;
		void(T::*m_SetFloatValue)(float); 
		float m_period;
		float m_amplitude;
		float m_offset;
		float m_minClamp;
		float m_maxClamp;
		float m_time;
	};

	template<class T, typename Vec2Type> 
	class Vec2Animator : public ISimpleAnimator
	{
	public:
		Vec2Animator(T* target, void(T::*SetVec2Value)(const Vec2Type&), float xValStart, float yValStart) :
			m_target(target),
			m_SetVec2Value(SetVec2Value)
		{
			m_vec2.x = xValStart;
			m_vec2.y = yValStart;
		}

		virtual void UpdateValue(float /*delta*/)
		{
			(m_target->*m_SetVec2Value)(m_vec2);
		}

		void SetX(float x)
		{
			m_vec2.x = x;
		}

		void SetY(float y)
		{
			m_vec2.y = y;
		}
	
	private:
		T* m_target;
		void(T::*m_SetVec2Value)(const Vec2Type&); 
		Vec2Type m_vec2;
	};
}