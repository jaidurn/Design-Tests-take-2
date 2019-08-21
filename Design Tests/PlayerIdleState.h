#pragma once
//==========================================================================================
// File Name: PlayerIdleState.h
// Author: Brian Blackmon
// Date Created: 5/13/2019
// Purpose: 
// Handles the logic for the Player's Idle State.
//==========================================================================================
#include "PlayerState.h"
#include <string>

class PlayerIdleState :	public PlayerState
{
public:
	PlayerIdleState(int entityID, std::string name);
	virtual ~PlayerIdleState();

	virtual void enter();
	virtual void update();
	virtual void exit();

private:
};

