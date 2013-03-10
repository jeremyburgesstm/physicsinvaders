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

namespace Helpers
{
	template<typename T>
	T Clamp(T val, T min, T max)
	{
		return Min(max, Max(val, min));
	}

	template<typename T>
	T Min(T val1, T val2)
	{
		return val1 <= val2 ? val1 : val2;
	}

	template<typename T>
	T Max(T val1, T val2)
	{
		return val1 >= val2 ? val1 : val2;
	}

	template<typename T>
	int Sign(T val)
	{
		return val > 0 ? 1 : (val < 0 ? -1 : 0);
	}

	template<typename T>
	T Lerp(T start, T end, float t)
	{
		return t * (end - start) + start;
	}

	// Following code borrowed directly from cplusplus.com, but it's a nice one!
	template <typename T, size_t N>
	inline
	size_t SizeOfArray( const T(&)[ N ] )
	{
	  return N;
	}
}