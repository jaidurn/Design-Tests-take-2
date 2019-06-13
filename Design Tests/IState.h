#pragma once
//==========================================================================================
// File Name: IState.h
// Author: Brian Blackmon
// Date Created: 5/13/2019
// Purpose: 
// The interface class for states.
//==========================================================================================
#include "Timer.h"
#include "PhysicsSystem.h"
#include "StateChangeMessage.h"
#include "AnimationChangeMessage.h"
#include "MessageSystem.h"
#include "EntitySystem.h"
#include "RenderSystem.h"
#include <iostream>

class IState
{
public:
	IState(int entityID)
		:m_entityID(entityID)
	{
	}

	virtual ~IState() {}

	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;
	
protected:
	int m_entityID;
	Timer m_timer;

	void sendStateChangeMessage(std::string stateName)
	{
		StateChangeMessage *state = new StateChangeMessage(m_entityID, stateName);
		MessageSystem::instance()->pushMessage(state);
	}

	void sendAnimationChangeMessage(std::string animationName, AnimationComponent::Direction direction, int frame)
	{
		AnimationChangeMessage *animation = new AnimationChangeMessage(m_entityID, animationName, direction, frame);
		MessageSystem::instance()->pushMessage(animation);
	}

	virtual void processMessage(IMessage *message)
	{
		// Use this to process collision events or other event types.
	}
};

