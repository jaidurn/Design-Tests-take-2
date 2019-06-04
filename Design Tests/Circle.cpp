#include "Circle.h"

using namespace Shape;

Circle::Circle()
	:IShape(0, 0), m_radius(0)
{

}

Circle::Circle(float centerX, float centerY, int radius)
	:IShape(centerX, centerY), m_radius(radius)
{

}

Circle::~Circle()
{

}