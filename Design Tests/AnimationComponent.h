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
	AnimationComponent();
	virtual ~AnimationComponent();

	void update();

	// Current information.
	std::string currentAnimation();
	SDL_Rect* currentFrame();
	float currentSpeed();
	bool currentLoop();

	// Length information.
	int currentFrameIndex();
	int currentAnimationLength();
	int animationLength(std::string name);

	// Adding animations/frames
	void addAnimation(std::string name, bool loop, float speed);
	void addFrame(std::string name, SDL_Rect frame);

	// Setting information.
	void setAnimation(std::string name);
	void setCurrentFrame(float frame);
	void setCurrentSpeed(float speed);
	void setAnimationSpeed(std::string name, float speed);
	void resetIndex();

private:
	std::map<std::string, Animation> m_animations;

	std::string m_currentName;
	float m_currentFrameIndex;
};

