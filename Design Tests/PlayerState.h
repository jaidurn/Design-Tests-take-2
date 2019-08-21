#pragma once
//==========================================================================================
// File Name: PlayerState.h
// Author: Brian Blackmon
// Date Created: 6/28/2019
// Purpose: 
// The base class for all player states.
//==========================================================================================
#include "IState.h"
#include "string"
#include "Timer.h"

class PlayerState :	public IState
{
public:
	PlayerState(int entityID, std::string stateName);
	virtual ~PlayerState();

	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;

	virtual void processMessage(IMessage *message);

protected:
	Timer m_timer;

	std::string m_name;
};

