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

		void setCenter(float x, float y) 
		{ 
			m_center.setX(x);
			m_center.setY(y);
		}

		virtual void setCenterX(float centerX) { m_center.setX(centerX); }
		virtual void setCenterY(float centerY) { m_center.setY(centerY); }
		void setRotation(float rotation) { m_rotation = rotation; }

		virtual Vector2D center() const { return m_center; }
		float rotation() const { return m_rotation; }

	protected:
		Vector2D m_center;
		float m_rotation;
	};
}