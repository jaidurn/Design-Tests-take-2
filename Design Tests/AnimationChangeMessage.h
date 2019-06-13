#pragma once
//==========================================================================================
// File Name: AnimationChangeMessage.h
// Author: Brian Blackmon
// Date Created: 5/20/2019
// Purpose: 
// Holds information about what animation to change to.
//==========================================================================================
#include "IMessage.h"
#include "AnimationComponent.h"
#include <string>

class AnimationChangeMessage : public IMessage
{
public:
	AnimationChangeMessage(int entityID, std::string name, AnimationComponent::Direction direction, int frame = -1)
		:IMessage(ANIMATION_CHANGE), m_entityID(entityID), m_name(name), m_frame(frame), m_direction(direction)
	{

	}

	virtual ~AnimationChangeMessage() {}

	int m_entityID;
	int m_frame;
	AnimationComponent::Direction m_direction;
	std::string m_name;
};