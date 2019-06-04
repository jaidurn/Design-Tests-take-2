#pragma once
//==========================================================================================
// File Name: Brian Blackmon
// Author: 
// Date Created: 
// Purpose: 
// 
//==========================================================================================
#include "IMessage.h"
#include "Vector2D.h"

class MoveMessage : public IMessage
{
public:
	MoveMessage(int entityID, Vector2D oldPosition, Vector2D newPosition)
		:IMessage(MOVE), m_entityID(entityID), m_oldPosition(oldPosition), m_newPosition(newPosition)
	{
	}
	virtual ~MoveMessage() {}

	int m_entityID;
	Vector2D m_oldPosition;
	Vector2D m_newPosition;
};

