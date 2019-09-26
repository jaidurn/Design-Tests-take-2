#include "UIYList.h"
#include "UIGraphic.h"
#include "UIButton.h"
#include "UIText.h"

#include "MessageSystem.h"
#include "InputMessage.h"
#include "EntityDestroyMessage.h"

#include <iostream>

UIYList::UIYList(int entityID,
	Vector2D position,
	int width,
	int height,
	int itemsVisible)
	:UIGroup(entityID, position, width, height),
	m_itemsVisible(itemsVisible),
	m_itemOffset(0),
	m_currentItem(0),
	m_prevUpdate(0)
{
	if (m_itemsVisible <= 0)
	{
		m_itemsVisible = 1;
	}
}

UIYList::~UIYList()
{
	cleanUp();
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the center position of the vertical list and moves the items in it.
// Parameters:
// Vector2D position - The new center position of the vertical list.
//=============================================================================
void UIYList::setPosition(Vector2D position)
{
	if (m_background)
	{
		m_background->setPosition(position);
	}

	if (m_rect)
	{
		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			Vector2D itemOffset = m_items[i]->getPosition() - m_rect->center();
			Vector2D itemPos = position + itemOffset;

			itemPos.setX(position.getX());

			m_items[i]->setPosition(itemPos);
		}

		m_rect->setCenter(position);

	}
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the vertical list and the width of its items.
// Parameters:
// int width - The new width of the vertical list.
//=============================================================================
void UIYList::setWidth(int width)
{
	if (m_background)
	{
		m_background->setWidth(width);
	}

	if (m_rect)
	{
		m_rect->setWidth(width);

		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			m_items[i]->setWidth(width);
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the vertical list and adjusts the height of its items.
// Parameters:
// int height - The new height of the vertical list.
//=============================================================================
void UIYList::setHeight(int height)
{
	if (m_background)
	{
		m_background->setHeight(height);
	}

	if (m_rect)
	{
		m_rect->setHeight(height);

		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			m_items[i]->setHeight(height / m_itemsVisible);
		}
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state of the vertical list and updates its items.
// Parameters:
// bool active - The new active state to set.
//=============================================================================
void UIYList::setActive(bool active)
{
	if (m_active != active)
	{
		UIComponent *currentItem = getCurrentItem();

		if (currentItem)
		{
			currentItem->setActive(active);
		}

		if (m_background)
		{
			m_background->setActive(active);
		}

		m_active = active;
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visible state of the vertical list and its items.
// Parameters:
// bool visible - The new visible state to set.
//=============================================================================
void UIYList::setVisible(bool visible)
{
	if (m_visible != visible)
	{
		if (!m_visible)
		{
			int startingItem = m_itemOffset;
			int endingItem = m_itemOffset + m_itemsVisible;

			if (m_items.size() < (unsigned int)endingItem)
			{
				endingItem = (int)m_items.size();
			}

			for (int i = startingItem; i < endingItem; i++)
			{
				m_items[i]->setVisible(visible);
			}
		}
		else
		{
			for (unsigned int i = 0; i < m_items.size(); i++)
			{
				if (m_items[i])
				{
					m_items[i]->setVisible(visible);
				}
			}
		}

		if (m_background)
		{
			m_background->setVisible(visible);
		}

		m_visible = visible;
	}
}

//=============================================================================
// Function: void addItem(UIComponent *item)
// Description:
// Adds a new item to the list. This sets its position, active state, and
// visible state as neccesary.
// Parameters:
// UIComponent *item - The item to add to the list.
//=============================================================================
void UIYList::addItem(UIComponent *item)
{
	if (item)
	{
		if (m_rect)
		{
			int itemHeight = m_rect->height() / m_itemsVisible;
			int itemWidth = m_rect->width();
			int itemIndex = (int)m_items.size();

			int yOffset = (int)m_rect->getTopLeft().getY() + (itemHeight * (itemIndex - m_itemOffset) + (itemHeight / 2));

			Vector2D itemPos(m_rect->center().getX(), (float)yOffset);

			item->setHeight(itemHeight);
			item->setWidth(itemWidth);
			item->setPosition(itemPos);

			if (itemIndex < m_itemOffset ||
				m_itemOffset + m_itemsVisible - 1 < itemIndex)
			{
				item->setVisible(false);
			}

			item->setActive(false);

			if (itemIndex == m_currentItem)
			{
				item->setActive(true);
			}

			m_items.emplace_back(item);

			updatePositions();
		}
		else
		{
			std::cout << "RECT DOESN'T EXIST!\n"
				<< "DELETING ITEM WITH NAME: "
				<< item->getName()
				<< "!\n";

			delete item;
			item = NULL;
		}
	}
}


UIComponent* UIYList::getCurrentItem()
{
	UIComponent *current = NULL;

	if (m_currentItem < (int)m_items.size())
	{
		current = m_items[m_currentItem];
	}

	return current;
}