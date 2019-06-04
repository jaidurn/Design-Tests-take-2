#pragma once
//==========================================================================================
// File Name: EnemyState.h
// Author: Brian Blackmon
// Date Created: 5/31/2019
// Purpose: 
// The base state for enemy attack states.
//==========================================================================================
#include "IState.h"
class EnemyState : public IState
{
public:
	enum Behavior
	{
		BEHAVIOR_NONE,
		BEHAVIOR_NORMAL,
		BEHAVIOR_AGGRESSIVE,
		BEHAVIOR_PASSIVE,
		BEHAVIOR_SLOW,
	};

	EnemyState(int entityID, float weight);
	virtual ~EnemyState();

	void setWeight(float weight) { m_weight = weight; }
	float getWeight() { return m_weight; }

	// These need to be defined by the states themselves.
	virtual bool canEnter() = 0;
	virtual bool canExit() = 0;

	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;

private:
	float m_weight;
};

