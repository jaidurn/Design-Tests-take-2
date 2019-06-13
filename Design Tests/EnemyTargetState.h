#pragma once
//==========================================================================================
// File Name: EnemyTargetState.h
// Author: Brian Blackmon
// Date Created: 6/10/2019
// Purpose: 
// Handles the logic for targeting.
//==========================================================================================
#include "EnemyState.h"
class EnemyTargetState : public EnemyState
{
public:
	EnemyTargetState(int entityID, float weight, bool lineOfSight, std::string targetType, int range);
	virtual ~EnemyTargetState();

	virtual bool canEnter(int targetID, Behavior behavior);
	virtual bool canExit();

	virtual void enter();
	virtual void update();
	virtual void exit();

	int currentTarget() { return m_currentTarget; }

private:
	std::string m_targetType;
	int m_range;
	int m_currentTarget;
	bool m_lineOfSight;
};

