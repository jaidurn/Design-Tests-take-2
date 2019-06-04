#pragma once
//==========================================================================================
// File Name: EnemyLogicComponent.h
// Author: Brian Blackmon
// Date Created: 5/30/2019
// Purpose: 
// Handles the enemy logic. Uses a priority queue.
//==========================================================================================
#include "LogicComponent.h"
#include <map>
#include <vector>

class EnemyLogicComponent :	public LogicComponent
{
public:
	EnemyLogicComponent();

	virtual ~EnemyLogicComponent();

	virtual void addState(std::string stateName, IState *state);
	// The default to use.
	void addState(std::string stateName, IState *state, float weight);

	virtual void removeState(std::string stateName);
	virtual IState* getState(std::string stateName);
	virtual IState* currentState();
	virtual void changeState(std::string stateName);

	virtual void update();

protected:	
	virtual void cleanUp();

private:
	std::map<std::string, IState*> m_states;
	std::vector<std::string> m_statesByValue;

	IState *m_currentState;

	void sortStatesByValue();
};

