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

#include "LogConstants.h"

// Required STL Includes
#include <string>
#include <vector>

#ifndef BUILD_RELEASE
	#define LOG(x, y, z) Log::Log(x, y, z)
#else
	#define LOG(x, y, z) 0
#endif

namespace Log
{
	class ILogTarget;

	void Log(Constants::Channel channel, Constants::Level level, const std::string& message);
	void Info(Constants::Channel channel, const std::string& message);
	void Debug(Constants::Channel channel, const std::string& message);
	void Warn(Constants::Channel channel, const std::string& message);
	void Error(Constants::Channel channel, const std::string& message);
	
	// Hack method for now to register a log target.
	void RegisterLogTarget(Constants::Channel channel, Constants::Level level, ILogTarget* newTarget);

	/// Initialise & Cleanup are basically singleton creation/destruction. Note that neither is threadsafe, and both
	/// should take place when only 1 thread is running.
	void Initialise();
	void Cleanup();

	class Logger
	{
	public:
		Logger();
		~Logger();

		void DoLog(Constants::Channel channel, Constants::Level level, const std::string& message) const;
		void AddTargetForChannel(Constants::Channel channel, Constants::Level level, ILogTarget* newTarget);

	private:
		struct ChannelDataContainer
		{
		public:
			ChannelDataContainer();
			~ChannelDataContainer();

			void AddTargetForLevel(Constants::Level level, ILogTarget* newTarget);
			const std::vector<ILogTarget*>* GetTargetsForLevel(Constants::Level level) const;

		private:
			std::vector<ILogTarget*>* m_levelTargets[Constants::LEVEL_COUNT];
		};

		ChannelDataContainer* m_channelMap[Constants::CHANNEL_COUNT];
	};
}
