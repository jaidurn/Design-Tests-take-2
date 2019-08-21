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
		LOGIC_BOSS,
		LOGIC_DOOR
	};

	LogicComponent(LogicType type, int entityID)
	:Component(LOGIC), m_type(type), m_entityID(entityID)
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

	virtual void processMessage(IMessage *message) = 0;

protected:
	int m_entityID;
	LogicType m_type;

	virtual void cleanUp() = 0;
};

