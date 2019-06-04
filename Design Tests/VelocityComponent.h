#pragma once
//==========================================================================================
// File Name: VelocityComponent.h
// Author: Brian Blackmon
// Date Created: 3/28/2019
// Purpose: 
// Holds velocity information.
//==========================================================================================
#include "Component.h"
#include "Vector2D.h"
class VelocityComponent : public Component
{
public:
	VelocityComponent();
	virtual ~VelocityComponent();

	float xSpeed() { return m_velocity.getX(); }
	float ySpeed() { return m_velocity.getY(); }
	Vector2D velocity() { return m_velocity; }

	void setHorizonalSpeed(float xSpeed) { m_velocity.setX(xSpeed); }
	void setVerticalSpeed(float ySpeed) { m_velocity.setY(ySpeed); }
	void increaseHorizontalSpeed(float xIncrease) { m_velocity.setX(m_velocity.getX() + xIncrease); }
	void increaseVerticalSpeed(float yIncrease) { m_velocity.setY(m_velocity.getY() + yIncrease); }

private:
	Vector2D m_velocity;
};

