#include "UIComponent.h"
#include "MessageSystem.h"
#include "MoveMessage.h"

UIComponent::UIComponent(int entityID,
	UI_Type type,
	Vector2D position,
	string name)
	:Component(UI),
	m_entityID(entityID),
	m_name(name),
	m_type(type),
	m_rect(NULL),
	m_visible(true)
{
	m_rect = new Rectangle(position.getX(),
						   position.getY(),
						   0,
						   0);
}

UIComponent::~UIComponent()
{
	if (m_rect)
	{
		delete m_rect;
		m_rect = NULL;
	}
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the components position if possible.
// Parameters:
// Vector2D position - The new position of the component.
//=============================================================================
void UIComponent::setPosition(Vector2D position)
{
	if (!m_rect)
	{
		m_rect = new Rectangle(position.getX(), position.getY(), 0, 0);
	}

	MoveMessage *move = new MoveMessage(m_entityID, m_rect->center(), position);

	MessageSystem::instance()->pushMessage(move);

	m_rect->setCenter(position.getX(), position.getY());
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the component if possible.
// Parameters:
// int width - The new width of the component.
//=============================================================================
void UIComponent::setWidth(int width)
{
	if (m_rect)
	{
		if (0 < width)
		{
			m_rect->setWidth(width);
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the component if possible.
// Parameters:
// int height - The new height of the component.
//=============================================================================
void UIComponent::setHeight(int height)
{
	if (m_rect)
	{
		if (0 < height)
		{
			m_rect->setHeight(height);
		}
	}
}

//=============================================================================
// Function: void setName(string)
// Description:
// Sets the name of the component if possible.
// Parameters:
// string name - The new name of the component.
//=============================================================================
void UIComponent::setName(string name)
{
	if (name != "")
	{
		m_name = name;
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visible state.
// Parameters:
// bool visible - The new visible state.
//=============================================================================
void UIComponent::setVisible(bool visible)
{
	m_visible = visible;
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state.
// Parameters:
// bool active - The new active state.
//=============================================================================
void UIComponent::setActive(bool active)
{
	m_active = active;
}

//=============================================================================
// Function: Vector2D getPosition()
// Description:
// Gets the center position of the component.
// Output:
// Vector2D
// On success - Returns a Vector2D object containing the center of the
//				component.
// On failure - Returns a Vector2D object containing the values
//				(-1, -1)
//=============================================================================
Vector2D UIComponent::getPosition()
{
	Vector2D position(-1, -1);

	if (m_rect)
	{
		position = m_rect->center();
	}

	return position;
}

//=============================================================================
// Function: int getWidth()
// Description:
// Gets the width of the component.
// Output:
// int
// On success - Returns the width of the component.
// On failure - Returns -1.
//=============================================================================
int UIComponent::getWidth()
{
	int width = -1;

	if (m_rect)
	{
		width = m_rect->width();
	}

	return width;
}

//=============================================================================
// Function: int getHeight()
// Description:
// Gets the height of the component.
// Output:
// int
// On success - Returns the height of the component.
// On failure - Returns -1.
//=============================================================================
int UIComponent::getHeight()
{
	int height = -1;

	if (m_rect)
	{
		height = m_rect->height();
	}

	return height;
}

//=============================================================================
// Function: string getName()
// Description:
// Gets the name of the component.
// Output:
// string
// Returns a string containing the name of the component.
//=============================================================================
string UIComponent::getName()
{
	return m_name;
}

//=============================================================================
// Function: bool getVisible()
// Description:
// Gets the current visible state.
// Output:
// bool
// Returns the visible state.
//=============================================================================
bool UIComponent::getVisible()
{
	return m_visible;
}

//=============================================================================
// Function: bool getActive()
// Description:
// Gets the current active state.
// Output:
// bool
// Returns the active state.
//=============================================================================
