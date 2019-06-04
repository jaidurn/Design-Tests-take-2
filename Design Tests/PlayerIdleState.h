#pragma once
//==========================================================================================
// File Name: PlayerIdleState.h
// Author: Brian Blackmon
// Date Created: 5/13/2019
// Purpose: 
// Handles the logic for the Player's Idle State.
//==========================================================================================
#include "IState.h"
#include <string>

class PlayerIdleState :	public IState
{
public:
	PlayerIdleState(int entityID, std::string name);
	virtual ~PlayerIdleState();

	virtual void enter();
	virtual void update();
	virtual void exit();

private:
	std::string m_name;
};

