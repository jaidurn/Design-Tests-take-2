#pragma once
//==========================================================================================
// File Name: EnemyIdleState.h
// Author: Brian Blackmon
// Date Created: 5/31/2019
// Purpose: 
// Holds the information required for the enemy's idle state.
//==========================================================================================
#include "EnemyState.h"

class EnemyIdleState : public EnemyState
{
public:
	EnemyIdleState(int entityID, float weight);
	virtual ~EnemyIdleState();

	virtual bool canEnter();
	virtual bool canExit();

	virtual void enter();
	virtual void update();
	virtual void exit();
};

