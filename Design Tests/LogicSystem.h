#pragma once
//==========================================================================================
// File Name: LogicSystem.h
// Author: Brian Blackmon
// Date Created: 5/22/2019
// Purpose: 
// Holds the information for creating and modifying Logic Components.
//==========================================================================================
#include "LogicComponent.h"
#include "IMessage.h"
#include <map>

class LogicSystem
{
public:
	static LogicSystem* instance()
	{
		static LogicSystem *logic = new LogicSystem();

		return logic;
	}

	~LogicSystem();

	LogicComponent* getLogicComponent(int entityID);
	LogicComponent* createLogicComponent(int entityID, LogicComponent::LogicType type);

	void processMessage(IMessage *message);
	void update();

private:
	LogicSystem()
	{

	}

	std::map<int, LogicComponent*> m_logicComponents;

	void cleanUp();
};

