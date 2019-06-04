#pragma once
//==========================================================================================
// File Name: AttackInfo.h
// Author: Brian Blackmon
// Date Created: 5/31/2019
// Purpose: 
// Holds information about an attack's collision boxes.
//==========================================================================================
#include "Rectangle.h"
#include <vector>

class AttackInfo
{
public:
	AttackInfo(float knockback, int range)
		:m_knockback(knockback), m_range(range)
	{

	}

	~AttackInfo()
	{
	}

	int range() { return m_range; }

	void addCollisionBox(Vector2D position, int width, int height)
	{
		Shape::Rectangle rect(position.getX(), position.getY(), width, height);

		m_collisionBoxes.push_back(rect);
	}

	Shape::Rectangle* getCollisionBox(int index)
	{
		Shape::Rectangle *rect = NULL;

		if(0 <= index && index < m_collisionBoxes.size())
		{
			rect = &m_collisionBoxes[index];
		}

		return rect;
	}

private:
	std::vector<Shape::Rectangle> m_collisionBoxes;
	float m_knockback;
	int m_range;
};