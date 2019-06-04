#include "Camera2D.h"



Camera2D::Camera2D(SDL_Rect box, float xScale, float yScale)
	:m_size(box), m_baseScaleX(xScale), m_baseScaleY(yScale)
{
	m_currentScaleX = m_baseScaleX;
	m_currentScaleY = m_baseScaleY;
}


Camera2D::~Camera2D()
{
}

//=============================================================================
// Function: void setCurrentScale(float, float)
// Description:
// Sets the current x and y scale of the camera.
// Parameters:
// float xScale - The scale to set the x to.
// float yScale - The scale to set the y to.
//=============================================================================
void Camera2D::setCurrentScale(float xScale, float yScale)
{
	if(0.5f <= xScale)
	{
		m_currentScaleX = xScale;
	}

	if(0.5f <= yScale)
	{
		m_currentScaleY = yScale;
	}
}