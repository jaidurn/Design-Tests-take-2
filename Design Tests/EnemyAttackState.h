#pragma once
//==========================================================================================
// File Name: EnemyAttackState.h
// Author: Brian Blackmon
// Date Created: 5/31/2019
// Purpose: 
// Holds all the information required for the enemy's attack.
// This information includes attack hitboxes, attack knockback, 
// 
//==========================================================================================
#include "EnemyState.h"
#include "AttackInfo.h"

class EnemyAttackState : public EnemyState
{
public:
	EnemyAttackState(int entityID, float weight);
	virtual ~EnemyAttackState();

	virtual bool canEnter(int targetID, Behavior behavior);
	virtual bool canExit();

	virtual void enter();
	virtual void update();
	virtual void exit();

private:
	bool m_attacked;
	Timer m_cooldownTimer;

	int m_currentTarget;
	int m_currentAttack;
};

