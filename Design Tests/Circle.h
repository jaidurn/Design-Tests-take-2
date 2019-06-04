#pragma once
//==========================================================================================
// File Name: Circle.h
// Author: Brian Blackmon
// Date Created: 4/13/2019
// Purpose: 
// Holds the information for a circle object.
//==========================================================================================
#include "IShape.h"

namespace Shape
{
	class Circle : public IShape
	{
	public:
		Circle();

		Circle(float centerX, float centerY, int radius);

		~Circle();

		virtual ShapeType type() { return CIRCLE; }

		void setRadius(const int radius) { m_radius = radius; }

		int radius() const { return m_radius; }

	private:
		int m_radius;
	};
}