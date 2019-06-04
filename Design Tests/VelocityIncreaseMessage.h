#pragma once
//==========================================================================================
// File Name: VelocityIncreaseMessage.h
// Author: Brian Blackmon
// Date Created: 5/14/2019
// Purpose: 
// Holds the information for increasing the velocity of a velocity component.
//==========================================================================================
#include "IMessage.h"

class VelocityIncreaseMessage : public IMessage
{
public:
	VelocityIncreaseMessage(int entityID, float xIncrease = 0, float yIncrease = 0, float xMaxSpeed = -1, float yMaxSpeed = -1)
		:IMessage(VELOCITY_INCREASE), m_entityID(entityID), m_xIncrease(xIncrease), m_yIncrease(yIncrease), m_xMaxSpeed(xMaxSpeed), m_yMaxSpeed(yMaxSpeed)
	{
	}
	virtual ~VelocityIncreaseMessage() {}

	int m_entityID;
	float m_xIncrease;
	float m_yIncrease;
	float m_xMaxSpeed;
	float m_yMaxSpeed;
};