#pragma once
//==========================================================================================
// File Name: ZombieIdleState.h
// Author: Brian Blackmon
// Date Created: 5/25/2019
// Purpose: 
// The logic for the zombie idle state.
//==========================================================================================
#include "IState.h"
class ZombieIdleState :	public IState
{
public:
	ZombieIdleState(int entityID);
	virtual ~ZombieIdleState();

	virtual void enter();
	virtual void update();
	virtual void exit();

private:
};

