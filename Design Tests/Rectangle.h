#pragma once
//==========================================================================================
// File Name: Rectangle.h
// Author: Brian Blackmon
// Date Created: 3/25/2019
// Purpose: 
// Holds information for a rectangle shape.
//==========================================================================================
#include "IShape.h"

namespace Shape
{
	class Rectangle : public IShape
	{
	public:
		Rectangle();

		Rectangle(float centerX, float centerY, int width, int height);

		~Rectangle();

		virtual ShapeType type() { return RECTANGLE; }

		void setWidth(const int width) { m_width = width; }
		void setHeight(const int height) { m_height = height; }

		virtual void setCenterX(float x) { m_center.setX(x); }
		virtual void setCenterY(float y) { m_center.setY(y); }

		int width() const { return m_width; }
		int height() const { return m_height; }

		Vector2D getTopRight();
		Vector2D getTopLeft();
		Vector2D getBottomRight();
		Vector2D getBottomLeft();

	private:
		int m_width;
		int m_height;
	};
}