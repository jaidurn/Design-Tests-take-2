#pragma once
//==========================================================================================
// File Name: Camera2D.h
// Author: Brian Blackmon
// Date Created: 5/21/2019
// Purpose: 
// Holds the information for a camera object.
//==========================================================================================
#include <SDL.h>

class Camera2D
{
public:
	Camera2D(SDL_Rect box, float xScale, float yScale);
	~Camera2D();

	SDL_Rect size() { return m_size; }
	
	float currentScaleX() { return m_currentScaleX; }
	float currentScaleY() { return m_currentScaleY; }
	float baseScaleX() { return m_baseScaleX; }
	float baseScaleY() { return m_baseScaleY; }

	void setWidth(int width) { m_size.w = 0; }
	void setHeight(int height) { m_size.h = 0; }

	void setCurrentScale(float xScale, float yScale);

private:
	float m_baseScaleX;
	float m_baseScaleY;
	float m_currentScaleX;
	float m_currentScaleY;

	SDL_Rect m_size;
};

