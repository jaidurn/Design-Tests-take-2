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
	AttackInfo(float knockback, int range, int maskCount)
		:m_knockback(knockback), m_range(range), m_maskCount(maskCount)
	{
		m_collisionMasks.resize(maskCount);
	}

	~AttackInfo()
	{
	}

	int range() { return m_range; }
	int maskCount() { return m_maskCount; }

	void addCollisionBox(int maskNumber, Vector2D position, int width, int height)
	{
		Shape::Rectangle rect(position.getX(), position.getY(), width, height);

		if (0 <= maskNumber && maskNumber < m_maskCount)
		{
			m_collisionMasks[maskNumber].push_back(rect);
		}
	}

	Shape::Rectangle* getCollisionBox(int maskNumber, int index)
	{
		Shape::Rectangle *rect = NULL;

		if(0 <= maskNumber && (unsigned int)maskNumber < m_collisionMasks.size())
		{
			if (0 <= index && (unsigned int)index < m_collisionMasks[maskNumber].size())
			{
				rect = &m_collisionMasks[maskNumber][index];
			}
		}

		return rect;
	}

private:
	std::vector<std::vector<Shape::Rectangle>> m_collisionMasks;
	float m_knockback;
	int m_maskCount;
	int m_range;
};