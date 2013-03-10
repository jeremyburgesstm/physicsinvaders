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

#include "Log.h"
#include "ILogTarget.h"

namespace Log
{

	static Log::Logger* s_logger = nullptr;

	void Log(Constants::Channel channel, Constants::Level level, const std::string& message)
	{
		// Early out if our static log instance hasn't been created.
		if (!s_logger) return;
		s_logger->DoLog(channel, level, message + '\n');
	}

	void Info(Constants::Channel channel, const std::string& message)
	{
		Log(channel, Log::Constants::LEVEL_INFO, message);
	}

	void Debug(Constants::Channel channel, const std::string& message)
	{
		Log(channel, Log::Constants::LEVEL_DEBUG, message);
	}

	void Warn(Constants::Channel channel, const std::string& message)
	{
		Log(channel, Log::Constants::LEVEL_WARN, message);
	}

	void Error(Constants::Channel channel, const std::string& message)
	{
		Log(channel, Log::Constants::LEVEL_ERROR, message);
	}

	void RegisterLogTarget(Constants::Channel channel, Constants::Level level, ILogTarget* newTarget)
	{
		if (!s_logger) return;
		s_logger->AddTargetForChannel(channel, level, newTarget);
	}

	void Initialise()
	{
		s_logger = new Log::Logger();
	}

	void Cleanup()
	{
		delete s_logger;
		s_logger = nullptr;
	}

	Logger::Logger()
	{		
		std::fill(m_channelMap, m_channelMap + Constants::CHANNEL_COUNT, nullptr);
	}

	Logger::~Logger()
	{
		for (unsigned int i = 0; i < Constants::CHANNEL_COUNT; ++i)
		{
			delete m_channelMap[i];
		}
	}

	void Logger::DoLog(Constants::Channel channel, Constants::Level level, const std::string& message) const
	{
		if (m_channelMap[channel])
		{
			if (const std::vector<ILogTarget*>* targets = m_channelMap[channel]->GetTargetsForLevel(level))
			{
				for each (ILogTarget* target in *targets)
				{
					target->OutputLog(channel, level, message);
				}
			}
		}
	}

	void Logger::AddTargetForChannel(Constants::Channel channel, Constants::Level level, ILogTarget* newTarget)
	{
		if (!m_channelMap[channel])
		{
			m_channelMap[channel] = new ChannelDataContainer();
		}
		m_channelMap[channel]->AddTargetForLevel(level, newTarget);
	}

	Logger::ChannelDataContainer::ChannelDataContainer()
	{
		std::fill(m_levelTargets, m_levelTargets + Constants::LEVEL_COUNT, nullptr);
	}

	Logger::ChannelDataContainer::~ChannelDataContainer()
	{
		for (unsigned int i = 0; i < Constants::CHANNEL_COUNT; ++i)
		{
			if (!m_levelTargets[i]) continue;
			for each(ILogTarget* target in *m_levelTargets[i])
			{
				delete target;
			}
			m_levelTargets[i]->clear();
			delete m_levelTargets[i];
		}
	}

	void Logger::ChannelDataContainer::AddTargetForLevel(Constants::Level level, ILogTarget* newTarget)
	{
		if (!m_levelTargets[level])
		{
			m_levelTargets[level] = new std::vector<ILogTarget*>();
		}
		m_levelTargets[level]->push_back(newTarget);
	}

	const std::vector<ILogTarget*>*  Logger::ChannelDataContainer::GetTargetsForLevel(Constants::Level level) const
	{
		return m_levelTargets[level];
	}
}