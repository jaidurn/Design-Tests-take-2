#pragma once
//==========================================================================================
// File Name: Camera2D.h
// Author: Brian Blackmon
// Date Created: 5/21/2019
// Purpose: 
// Holds the information for a camera object.
//==========================================================================================
#include <SDL.h>
#include "Rectangle.h"

class Camera2D
{
public:
	Camera2D(SDL_Rect box, float xScale, float yScale);
	~Camera2D();

	SDL_Rect size() { return m_size; }
	Shape::Rectangle* boundingBox() { return m_boundingBox; }
	
	float currentScaleX() { return m_currentScaleX; }
	float currentScaleY() { return m_currentScaleY; }
	float baseScaleX() { return m_baseScaleX; }
	float baseScaleY() { return m_baseScaleY; }
	int currentTarget() { return m_currentTarget; }
	int getX() { return m_size.x; }
	int getY() { return m_size.y; }
	int getWidth() { return m_size.w; }
	int getHeight() { return m_size.h; }
	
	void setX(int x);
	void setY(int y);
	void setWidth(int width) { m_size.w = 0; }
	void setHeight(int height) { m_size.h = 0; }
	void setCurrentTarget(int target) { if (0 <= target) { m_currentTarget = target; } }

	void setCurrentScale(float xScale, float yScale);

	void setBoundingBoxSize(int width, int height);

private:
	float m_baseScaleX;
	float m_baseScaleY;
	float m_currentScaleX;
	float m_currentScaleY;

	int m_currentTarget;

	Shape::Rectangle *m_boundingBox;

	SDL_Rect m_size;
};

