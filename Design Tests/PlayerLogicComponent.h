#pragma once
//==========================================================================================
// File Name: PlayerLogicComponent.h
// Author: Brian Blackmon
// Date Created: 5/25/2019
// Purpose: 
// Holds information for player logic.
//==========================================================================================
#include "LogicComponent.h"
#include <map>

class PlayerLogicComponent : public LogicComponent
{
public:
	PlayerLogicComponent(LogicType type)
		:LogicComponent(type), m_currentStateName(""), m_currentState(NULL)
	{

	}

	virtual ~PlayerLogicComponent();

	virtual void addState(std::string stateName, IState *state);
	virtual void removeState(std::string stateName);
	virtual IState* getState(std::string stateName);
	virtual IState* currentState();
	virtual void changeState(std::string stateName);

	virtual void update();

protected:
	virtual void cleanUp();

private:
	std::string m_currentStateName;
	IState *m_currentState;
	std::map<std::string, IState*> m_states;
};

