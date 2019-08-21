#pragma once
//==========================================================================================
// File Name: Timer.h
// Author: Brian Blackmon
// Date Created: 5/31/2019
// Purpose: 
// Holds information for creating timers.
//==========================================================================================
#include <SDL.h>

class Timer
{
public:
	Timer()
		:m_running(false), m_paused(false), m_startTick(0), m_pausedTick(0)
	{

	}

	~Timer()
	{

	}

	bool running()
	{
		return m_running;
	}

	bool paused()
	{
		return m_paused;
	}

	void start()
	{
		m_running = true;
		m_paused = false;

		m_startTick = SDL_GetTicks();
		m_pausedTick = 0;
	}

	void pause()
	{
		if(m_running && !m_paused)
		{
			m_paused = true;
			m_pausedTick = SDL_GetTicks();
		}
	}

	void unpause()
	{
		if(m_running && m_paused)
		{
			m_paused = false;
			m_pausedTick = 0;
		}
	}

	void stop()
	{
		m_running = false;
		m_paused = false;
		m_startTick = 0;
		m_pausedTick = 0;
	}

	Uint32 currentTicks()
	{
		Uint32 current = 0;

		if(m_running)
		{
			if(m_paused)
			{
				current = SDL_GetTicks() - m_pausedTick;
			}
			else
			{
				current = SDL_GetTicks() - m_startTick;
			}
		}

		return current;
	}

	float currentSeconds()
	{
		float current = 0.0f;

		if(m_running)
		{
			if(m_paused)
			{
				float ticks = (float)(SDL_GetTicks() - m_pausedTick);
				current = ticks / (float)m_ticksPerSecond;
			}
			else
			{
				float ticks = (float)(SDL_GetTicks() - m_startTick);
				current = ticks / (float)m_ticksPerSecond;
			}
		}

		return current;
	}

private:
	const int m_ticksPerSecond = 1000;
	
	Uint32 m_startTick;
	Uint32 m_pausedTick;
	bool m_running;
	bool m_paused;
};