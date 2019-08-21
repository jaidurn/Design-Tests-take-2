#pragma once
//==========================================================================================
// File Name: CameraMoveMessage.h
// Author: Brian Blackmon
// Date Created: 8/17/2019
// Purpose: 
// Holds information about a camera's movement.
//==========================================================================================
#include "IMessage.h"
#include "Vector2D.h"

class CameraMoveMessage : public IMessage
{
public:
	CameraMoveMessage(Vector2D oldPosition, Vector2D newPosition)
		:IMessage(CAMERA_MOVE), 
		m_oldPosition(oldPosition),
		m_newPosition(newPosition)
	{

	}

	~CameraMoveMessage()
	{

	}

	Vector2D m_oldPosition;
	Vector2D m_newPosition;
};