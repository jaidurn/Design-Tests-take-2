#pragma once
//==========================================================================================
// File Name: LogicComponent.h
// Author: Brian Blackmon
// Date Created: 5/13/2019
// Purpose: 
// Base class for logic components.
//==========================================================================================
#include "Component.h"
#include "IState.h"
#include <map>
#include <string>

class LogicComponent : public Component
{
public:
	enum LogicType
	{
		LOGIC_PLAYER,
		LOGIC_ENEMY,
		LOGIC_BOSS
	};

	LogicComponent(LogicType type)
	:Component(LOGIC)
	{
	}

	virtual ~LogicComponent()
	{
	}

	LogicType type() { return m_type; }

	virtual void addState(std::string stateName, IState *state) = 0;
	virtual void removeState(std::string stateName) = 0;
	virtual IState* getState(std::string stateName) = 0;
	virtual IState* currentState() = 0;
	virtual void changeState(std::string stateName) = 0;

	virtual void update() = 0;

protected:
	virtual void cleanUp() = 0;

	LogicType m_type;
};

