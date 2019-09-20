#include "UIGroup.h"
#include "UIComponent.h"
#include "UIGraphic.h"
#include "UIButton.h"
#include "MessageSystem.h"
#include "InputMessage.h"
#include <iostream>

UIGroup::UIGroup(int entityID, 
	Vector2D position, 
	int width, 
	int height)
	:m_rect(NULL),
	m_background(NULL),
	m_entityID(entityID),
	m_active(true),
	m_visible(true)
{
	m_rect = new Rectangle(position, width, height);

	if (!m_rect)
	{
		std::cout << "Failed to create the rect for entity: "
			<< m_entityID << "!\n";
	}
}

UIGroup::~UIGroup()
{
	cleanUp();
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the center position for the group and moves the items.
// Parameters:
// Vector2D position - The new center position of the group.
//=============================================================================
void UIGroup::setPosition(Vector2D position)
{
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		Vector2D offset = m_items[i]->getPosition() - m_rect->center();

		m_items[i]->setPosition(position + offset);
	}

	if (m_background)
	{
		m_background->setPosition(position);
	}

	m_rect->setCenter(position);
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the group and adjusts everything inside it.
// Parameters:
// int width - The new width of the group.
//=============================================================================
void UIGroup::setWidth(int width)
{
	if (m_rect)
	{
		int widthDifference = m_rect->width() - width;

		if (widthDifference != 0)
		{
			if (widthDifference < 0) { widthDifference *= -1; }

			for (unsigned int i = 0; i < m_items.size(); i++)
			{
				Vector2D itemPos = m_items[i]->getPosition();
				int xFix = widthDifference / 2;

				if (itemPos.getX() < m_rect->center().getX())
				{
					xFix *= -1;
				}
				else if ((int)itemPos.getX() == (int)m_rect->center().getX())
				{
					xFix = 0;
				}

				itemPos.setX(itemPos.getX() + (float)xFix);

				m_items[i]->setPosition(itemPos);
			}

			if (m_background)
			{
				m_background->setWidth(width);
			}

			m_rect->setWidth(width);
		}
	}
}

//=============================================================================
// Function: void setHeight(int) 
// Description:
// Sets the height of the group and moves the items.
// Parameters:
// int height - The new height of the group.
//=============================================================================
void UIGroup::setHeight(int height)
{
	if (m_rect)
	{
		int heightDiff = m_rect->height() - height;

		if (heightDiff != 0)
		{
			if (heightDiff < 0) { heightDiff *= -1; }

			for (unsigned int i = 0; i < m_items.size(); i++)
			{
				Vector2D itemPos = m_items[i]->getPosition();
				
				int yFix = heightDiff / 2;

				if (itemPos.getY() < m_rect->center().getY())
				{
					yFix *= -1;
				}
				// Static cast to int for rounding.
				else if ((int)itemPos.getY() == (int)m_rect->center().getY())
				{
					yFix = 0;
				}

				itemPos.setY(itemPos.getY() + yFix);

				m_items[i]->setPosition(itemPos);
			}

			if (m_background)
			{
				m_background->setHeight(height);
			}

			m_rect->setHeight(height);
		}
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state of the group and items.
// Parameters:
// bool active - The new active state to set.
//=============================================================================
void UIGroup::setActive(bool active)
{
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i])
		{
			m_items[i]->setActive(active);
		}
	}

	if (m_background)
	{
		m_background->setActive(active);
	}

	m_active = active;
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visible state of the group and its items.
// Parameters:
// bool visible - The new visible state to set.
//=============================================================================
void UIGroup::setVisible(bool visible)
{
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i])
		{
			m_items[i]->setVisible(visible);
		}
	}

	if (m_background)
	{
		m_background->setVisible(visible);
	}

	m_visible = visible;
}

//=============================================================================
// Function: void setBackground(UIGraphic*)
// Description:
// Sets the current background to the specified graphic.
// Deletes the old background if there is one.
// Parameters:
// UIGraphic *background - The new background graphic.
//=============================================================================
void UIGroup::setBackground(UIGraphic *background)
{
	if (m_background)
	{
		delete m_background;
		m_background = NULL;
	}

	m_background = background;

	if (m_rect)
	{
		m_background->setPosition(m_rect->center());
		m_background->setWidth(m_rect->width());
		m_background->setHeight(m_rect->height());
	}
}

//=============================================================================
// Function: Vector2D getPosition()
// Description:
// Gets the center position of the group.
// Output:
// Vector2D
// On success - Returns a Vector2D object containing the center position.
// On failure - Returns a Vector2D object containing (-1, -1)
//=============================================================================
Vector2D UIGroup::getPosition()
{
	Vector2D centerPos(-1, -1);

	if (m_rect)
	{
		centerPos = m_rect->center();
	}

	return centerPos;
}

//=============================================================================
// Function: int getWidth()
// Description:
// Gets the width of the group.
// Output:
// int
// On success - Returns the width of the group.
// On failure - Returns -1.
//=============================================================================
int UIGroup::getWidth()
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
// Gets the height of the group.
// Output:
// int
// On success - Returns the height of the group.
// On failure - Returns -1.
//=============================================================================
int UIGroup::getHeight()
{
	int height = -1;

	if (m_rect)
	{
		height = m_rect->height();
	}

	return height;
}

//=============================================================================
// Function: bool getActive()
// Description:
// Gets the active state of the group.
// Output:
// bool
// Returns the active state of the group.
//=============================================================================
bool UIGroup::getActive()
{
	return m_active;
}

//=============================================================================
// Function: bool getVisible()
// Description:
// Gets the visible state of the group.
// Output:
// bool
// Returns the visible state.
//=============================================================================
bool UIGroup::getVisible()
{
	return m_visible;
}

//=============================================================================
// Function: void addItem(UIComponent*)
// Description:
// Adds an item to the list of items.
// Parameters:
// UIComponent *item - The item to add.
//=============================================================================
void UIGroup::addItem(UIComponent *item)
{
	if (item)
	{
		item->setActive(m_active);
		item->setVisible(m_visible);

		if (m_rect)
		{
			item->setPosition(item->getPosition() + m_rect->center());
		}

		m_items.emplace_back(item);
	}
}

//=============================================================================
// Function: void removeItem(int)
// Description:
// Finds and removes the component with the entityID.
// Parameters:
// int entityID - The ID of the component to remove.
//=============================================================================
void UIGroup::removeItem(int entityID)
{
	unsigned int i = 0;
	auto vectorIt = m_items.begin();

	while (i < m_items.size() - 1 && m_items[i]->getEntityID() != entityID)
	{
		vectorIt++;
		i++;
	}

	if (m_items[i]->getEntityID() == entityID)
	{
		delete m_items[i];
		m_items[i] = NULL;

		vectorIt = m_items.erase(vectorIt);
	}
}

//=============================================================================
// Function: void removeItem(string)
// Description:
// Finds and removes the component with the specified name.
// Parameters:
// string itemName - The name of the component to remove.
//=============================================================================
void UIGroup::removeItem(string itemName)
{
	unsigned int i = 0;
	auto vectorIt = m_items.begin();

	while (i < m_items.size() - 1 && m_items[i]->getName() != itemName)
	{
		vectorIt++;
		i++;
	}

	if (m_items[i]->getName() == itemName)
	{
		delete m_items[i];
		m_items[i] = NULL;

		vectorIt = m_items.erase(vectorIt);
	}
}

//=============================================================================
// Function: UIComponent* getItem(int)
// Description:
// Searches for and returns the component with the specified ID.
// Parameters:
// int entityID - The ID of the component to find.
// Output:
// UIComponent*
// On success - Returns a pointer to the found component.
// On failure - Returns NULL.
//=============================================================================
UIComponent* UIGroup::getItem(int entityID)
{
	UIComponent *item = NULL;

	unsigned int i = 0;

	while (i < m_items.size() - 1 && m_items[i]->getEntityID() != entityID)
	{
		i++;
	}

	if (m_items[i]->getEntityID() == entityID)
	{
		item = m_items[i];
	}

	return item;
}

//=============================================================================
// Function: UIComponent* getItem(string)
// Description:
// Searches for and returns the component with the specified name.
// Parameters:
// string itemName - The name of the component to find.
// Output:
// UIComponent*
// On success - Returns a pointer to the found component.
// On failure - Returns NULL.
//=============================================================================
UIComponent* UIGroup::getItem(string itemName)
{
	UIComponent *item = NULL;

	unsigned int i = 0;
	
	while (i < m_items.size() - 1 && m_items[i]->getName() != itemName)
	{
		i++;
	}

	if (m_items[i]->getName() == itemName)
	{
		item = m_items[i];
	}

	return item;
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes messages and handles input.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIGroup::processMessage(IMessage *message)
{
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i]->getType() == UIComponent::UI_BUTTON)
		{
			UIButton *button = static_cast<UIButton*>(m_items[i]);

			button->processMessage(message);
		}
	}
}

//=============================================================================
// Function: void update()
// Description:
// Updates the group and all of its components.
//=============================================================================
void UIGroup::update()
{
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i]->getType() == UIComponent::UI_BUTTON)
		{
			UIButton *button = static_cast<UIButton*>(m_items[i]);

			button->update();
		}
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up all of the items and frees the memory.
//=============================================================================
void UIGroup::cleanUp()
{
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		delete m_items[i];
	}

	m_items.clear();

	if (m_background)
	{
		delete m_background;
		m_background = NULL;
	}

	if (m_rect)
	{
		delete m_rect;
		m_rect = NULL;
	}
}