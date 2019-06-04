#pragma once
//==========================================================================================
// File Name: StateChangeMessage.h
// Author: Brian Blackmon
// Date Created: 5/13/2019
// Purpose: 
// Holds state change information.
//==========================================================================================
#include "IMessage.h"
#include <string>

class StateChangeMessage : public IMessage
{
public:
	StateChangeMessage(int entityID, std::string stateName)
		:IMessage(STATE_CHANGE), m_entityID(entityID), m_stateName(stateName)
	{
	}
	virtual ~StateChangeMessage() {}

	int m_entityID;
	std::string m_stateName;
};