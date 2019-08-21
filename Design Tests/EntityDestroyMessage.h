#pragma once
//==========================================================================================
// File Name: EntityDestroyMessage.h
// Author: Brian Blackmon
// Date Created: 6/24/2019
// Purpose: 
// Holds the information for destroying an entity.
//==========================================================================================
#include "IMessage.h"

class EntityDestroyMessage : public IMessage
{
public:
	EntityDestroyMessage(int entityID)
		:IMessage(ENTITY_DESTROY), m_entityID(entityID)
	{

	}

	virtual ~EntityDestroyMessage()
	{

	}

	int m_entityID;
};