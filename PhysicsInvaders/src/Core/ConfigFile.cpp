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

// My header
#include "ConfigFile.h"

// Std
#include <stdlib.h>     /* atoi */

// STL
#include <iostream>
#include <fstream>
#include <string>

// Boost
#include <boost/numeric/conversion/cast.hpp>

using namespace std;

ConfigFile::ConfigFile(const char* fileName) :
	m_isFullscreen(false),
	m_screenWidth(1280),
	m_screenHeight(800)
{
	string line;
	ifstream config(fileName);

	if (config.is_open())
	{
		while (config.good())
		{
			getline(config, line);
			if (line.find("screenwidth") != line.npos)
			{
				m_screenWidth =  boost::numeric_cast<unsigned short>(ParseIntValue(line.c_str()));
			}
			else if (line.find("screenheight") != line.npos)
			{
				m_screenHeight = boost::numeric_cast<unsigned short>(ParseIntValue(line.c_str()));
			}
			else if (line.find("fullscreen") != line.npos)
			{
				m_isFullscreen = true;
			}
		}
	}
}

int ConfigFile::ParseIntValue(const char* keyString)
{
	const char* colon = strchr(keyString, ':');
	return atoi(colon + 1);
}