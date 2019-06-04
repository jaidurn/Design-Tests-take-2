#include "Vector2D.h"



Vector2D::Vector2D(float x, float y)
	:m_x(x), m_y(y)
{
}


Vector2D::~Vector2D()
{
}

Vector2D absoluteValue(Vector2D a)
{
	float x = a.getX();
	float y = a.getY();

	if(a.getX() < 0)
	{
		x = a.getX() * -1;
	}

	if(a.getY() < 0)
	{
		y = a.getY() * -1;
	}

	return Vector2D(x, y);
}
