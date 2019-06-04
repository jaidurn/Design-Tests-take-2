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
	Animation(bool loop, float speed);
	~Animation();

	void addFrame(SDL_Rect frame);

	void setSpeed(float speed) { m_speed = speed; }
	void setLoop(bool loop) { m_loop = loop; }

	bool loop() { return m_loop; }
	float speed() { return m_speed; }

	int frameCount() { return m_frameCount; }
	SDL_Rect* getFrame(int frameIndex);

private:
	bool m_loop;
	float m_speed;
	int m_frameCount;

	std::vector<SDL_Rect> m_frames;
};

