#pragma once
//==========================================================================================
// File Name: Vector2D.h
// Author: Brian Blackmon
// Date Created: 4/2/2019
// Purpose: 
// Holds two-dimensional vector information.
//==========================================================================================
class Vector2D
{
public:
	Vector2D(float x, float y);
	~Vector2D();

	// Public Functions
	void setX(const float x) { m_x = x; }
	void setY(const float y) { m_y = y; }

	float getX() const { return m_x; }
	float getY() const { return m_y; }

	// Vector operator overloads
	friend Vector2D operator+(Vector2D a, Vector2D b)
	{
		return Vector2D(a.m_x + b.m_x, a.m_y + b.m_y);
	}

	friend Vector2D operator-(Vector2D a, Vector2D b)
	{
		return Vector2D(a.m_x - b.m_x, a.m_y - b.m_y);
	}

	friend Vector2D operator*(Vector2D a, Vector2D b)
	{
		return Vector2D(a.m_x * b.m_x, a.m_y * b.m_y);
	}

	friend bool operator==(Vector2D a, Vector2D b)
	{
		return (a.m_x == b.m_x && a.m_y == b.m_y);
	}
	
	friend bool operator!=(Vector2D a, Vector2D b)
	{
		return (a.m_x != b.m_x || a.m_y != b.m_y);
	}

	// Vector and other operator overloads
	friend Vector2D operator*(Vector2D a, float b)
	{
		return Vector2D(a.m_x * b, a.m_y * b);
	}

private:
	float m_x;
	float m_y;
};

Vector2D absoluteValue(Vector2D a);