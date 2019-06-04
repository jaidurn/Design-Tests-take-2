#pragma once
//==========================================================================================
// File Name: IShape.h
// Author: Brian Blackmon
// Date Created: 3/25/2019
// Purpose: 
// A shape interface for collision objects
//==========================================================================================
#include "Vector2D.h"

namespace Shape
{
	enum ShapeType
	{
		NONE,
		RECTANGLE,
		CIRCLE
	};

	class IShape
	{
	public:
		IShape()
			:m_center(0, 0), m_rotation(0.0)
		{
		}

		IShape(float centerX, float centerY)
			:m_center(centerX, centerY), m_rotation(0.0)
		{
		}

		~IShape() { }


		virtual ShapeType type() { return NONE; }

		void setCenter(const float x, const float y) 
		{ 
			m_center.setX(x);
			m_center.setY(y);
		}

		void setCenterX(const float centerX) { m_center.setX(centerX); }
		void setCenterY(const float centerY) { m_center.setY(centerY); }
		void setRotation(const float rotation) { m_rotation = rotation; }

		Vector2D center() const { return m_center; }
		float rotation() const { return m_rotation; }

	protected:
		Vector2D m_center;
		float m_rotation;
	};
}