#include "Rectangle.h"
#include "Rotation.h"

using namespace Shape;

Rectangle::Rectangle()
	:IShape(0, 0), m_width(0), m_height(0)
{

}

Rectangle::Rectangle(float centerX, 
	float centerY, 
	int width, 
	int height)
	:IShape(centerX, centerY), 
	m_width(width), 
	m_height(height)
{

}

Rectangle::Rectangle(Vector2D centerPos, int width, int height)
	:IShape(centerPos), m_width(width), m_height(height)
{

}

Rectangle::~Rectangle()
{

}

Vector2D Rectangle::getTopRight()
{
	Vector2D topRight(m_center.getX() + (m_width / 2), m_center.getY() - (m_height / 2));

	if (m_rotation != 0.0f)
	{
		topRight = rotatePoint(topRight, m_center, m_rotation);
	}

	return topRight;
}

Vector2D Rectangle::getTopLeft()
{
	Vector2D topLeft(m_center.getX() - (m_width / 2), m_center.getY() - (m_height / 2));

	if (m_rotation != 0.0f)
	{
		topLeft = rotatePoint(topLeft, m_center, m_rotation);
	}

	return topLeft;
}

Vector2D Rectangle::getBottomRight()
{
	Vector2D bottomRight(m_center.getX() + (m_width / 2), m_center.getY() + (m_height / 2));

	if (m_rotation != 0.0f)
	{
		bottomRight = rotatePoint(bottomRight, m_center, m_rotation);
	}

	return bottomRight;
}

Vector2D Rectangle::getBottomLeft()
{
	Vector2D bottomLeft(m_center.getX() - (m_width / 2), m_center.getY() + (m_height / 2));

	if (m_rotation != 0.0f)
	{
		bottomLeft = rotatePoint(bottomLeft, m_center, m_rotation);
	}

	return bottomLeft;
}