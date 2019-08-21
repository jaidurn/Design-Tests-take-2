#include "Camera2D.h"

Camera2D::Camera2D(SDL_Rect box, float xScale, float yScale)
	:m_size(box), m_baseScaleX(xScale), m_baseScaleY(yScale), m_boundingBox(NULL), m_currentTarget(-1)
{
	m_currentScaleX = m_baseScaleX;
	m_currentScaleY = m_baseScaleY;

	float bBoxX = (float)(box.x + (box.w / 2));
	float bBoxY = (float)(box.y + (box.h / 2));

	m_boundingBox = new Shape::Rectangle(bBoxX, bBoxY, box.w, box.h);
}


Camera2D::~Camera2D()
{
	if (m_boundingBox) { delete m_boundingBox; }

	m_boundingBox = NULL;
}

//=============================================================================
// Function: void setX(int)
// Description:
// Sets the x position of the camera and bounding box.
// Parameters:
// int x - The new x position.
//=============================================================================
void Camera2D::setX(int x)
{
	m_size.x = x;

	m_boundingBox->setCenterX((float)(m_size.x + (m_size.w / 2)));
}

//=============================================================================
// Function: void setY(int)
// Description:
// Sets the y position of the camera and bounding box.
// Parameters:
// int y - The new y position.
//=============================================================================
void Camera2D::setY(int y)
{
	m_size.y = y;

	m_boundingBox->setCenterY((float)(m_size.y + (m_size.h / 2)));
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

//=============================================================================
// Function: void setBoundingBoxSize(int, int)
// Description:
// Sets the size of the bounding box for the camera.
// Parameters:
// int width - The new width of the bounding box.
// int height - The new height of the bounding box.
//=============================================================================
void Camera2D::setBoundingBoxSize(int width, int height)
{
	m_boundingBox->setWidth(width);
	m_boundingBox->setHeight(height);
}