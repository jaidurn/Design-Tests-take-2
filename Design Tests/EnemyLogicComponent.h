#pragma once
//==========================================================================================
// File Name: EnemyLogicComponent.h
// Author: Brian Blackmon
// Date Created: 5/30/2019
// Purpose: 
// Handles the enemy logic. Uses a priority queue.
//==========================================================================================
#include "LogicComponent.h"
#include "EnemyState.h"
#include <map>
#include <vector>

class EnemyLogicComponent :	public LogicComponent
{
public:
	EnemyLogicComponent(int entityID);

	virtual ~EnemyLogicComponent();

	virtual void addState(std::string stateName, IState *state);
	// The default to use.
	void addState(std::string stateName, IState *state, float weight);

	void setBehavior(EnemyState::Behavior behavior);

	virtual void removeState(std::string stateName);
	virtual IState* getState(std::string stateName);
	virtual IState* currentState();
	virtual void changeState(std::string stateName);

	virtual void update();

	virtual void processMessage(IMessage *message);

protected:	
	virtual void cleanUp();

private:
	std::map<std::string, IState*> m_states;
	std::vector<std::string> m_statesByValue;

	std::string m_currentStateName;
	IState *m_currentState;

	EnemyState::Behavior m_behavior;
	int m_currentTarget;

	void sortStatesByValue();
};

