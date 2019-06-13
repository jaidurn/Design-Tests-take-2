#pragma once
//==========================================================================================
// File Name: Animation.h
// Author: Brian Blackmon
// Date Created: 5/20/2019
// Purpose: 
// Holds the information about an animation.
//==========================================================================================
#include <SDL.h>
#include <vector>

class Animation
{
public:
	Animation(bool loop, float speed, int directionCount);
	~Animation();

	void addFrame(int direction, SDL_Rect frame);

	void setSpeed(float speed) { m_speed = speed; }
	void setLoop(bool loop) { m_loop = loop; }

	bool loop() { return m_loop; }
	float speed() { return m_speed; }

	int frameCount(int direction);
	SDL_Rect* getFrame(int direction, int frameIndex);

private:
	bool m_loop;
	float m_speed;
	int m_directionCount;

	std::vector<std::vector<SDL_Rect>> m_frames;
};

