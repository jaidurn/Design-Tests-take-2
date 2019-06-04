#pragma once
//==========================================================================================
// File Name: CollisionMessage.h
// Author: Brian Blackmon
// Date Created: 5/13/2019
// Purpose: 
// Holds the information for the collision messages.
//==========================================================================================
#include "IMessage.h"
#include "Vector2D.h"

class CollisionMessage : public IMessage
{
public:
	CollisionMessage(int entityID, int collidingID, Vector2D position)
		:IMessage(COLLISION), m_entityID(entityID), m_collidingID(collidingID), m_position(position)
	{
	}
	virtual ~CollisionMessage() {}

	int m_entityID;
	int m_collidingID;
	Vector2D m_position;
};

