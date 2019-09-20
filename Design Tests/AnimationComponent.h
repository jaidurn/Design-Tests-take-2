#pragma once
//==========================================================================================
// File Name: AnimationComponent.h
// Author: Brian Blackmon
// Date Created: 5/16/2019
// Purpose: 
// Holds the information for an animation.
//==========================================================================================
#include "Component.h"
#include "Animation.h"
#include <SDL.h>
#include <map>
#include <vector>
#include <string>

class AnimationComponent : public Component
{
public:
	enum Direction
	{
		DIR_NONE = -1,
		DIR_DOWN = 0,
		DIR_RIGHT = 1,
		DIR_UP = 2,
		DIR_LEFT = 3,
		DIR_MAX = 4
	};

	AnimationComponent();
	virtual ~AnimationComponent();

	void update();

	// Current information.
	std::string currentAnimation();
	Direction currentDirection() { return m_currentDirection; }
	SDL_Rect* currentFrame();
	float currentSpeed();
	bool currentLoop();

	// Length information.
	int currentFrameIndex();
	int currentAnimationLength();
	int animationLength(std::string name);

	bool getPaused() { return m_paused; }

	// Adding animations/frames
	void addAnimation(std::string name, bool loop, float speed, int directionCount);
	void addFrame(std::string name, Direction direction, SDL_Rect frame);

	// Setting information.
	void setAnimation(std::string name);
	void setCurrentDirection(Direction direction);
	void setCurrentFrame(float frame);
	void setCurrentSpeed(float speed);
	void setAnimationSpeed(std::string name, float speed);
	void resetIndex();

	void setPaused(bool paused);

private:
	std::map<std::string, Animation> m_animations;

	std::string m_currentName;
	Direction m_currentDirection;
	float m_currentFrameIndex;
	bool m_paused;
};

