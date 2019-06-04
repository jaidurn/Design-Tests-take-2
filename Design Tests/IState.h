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
#include <iostream>

class IState
{
public:
	IState()
	{
	}

	virtual ~IState() {}
};

