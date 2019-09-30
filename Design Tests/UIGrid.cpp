#include "UIGrid.h"

#include "UIGraphic.h"
#include "UIButton.h"

#include "MessageSystem.h"
#include "InputMessage.h"
#include "EntityDestroyMessage.h"

#include "InputDevice.h"

UIGrid::UIGrid(int entityID,
	Vector2D position,
	int width,
	int height,
	int itemsPerRow,
	int rowCount)
	:UIGroup(entityID, position, width, height),
	m_itemsPerRow(itemsPerRow),
	m_rowCount(rowCount),
	m_rowOffset(0)
{

}

UIGrid::~UIGrid()
{
	cleanUp();
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the center position of the grid and updates the items.
// Parameters:
// Vector2D position - The new center position of the grid.
//=============================================================================
void UIGrid::setPosition(Vector2D position)
{
	if (m_rect)
	{
		if (m_background)
		{
			m_background->setPosition(position);
		}

		m_rect->setCenter(position);

		updatePositions();
	}
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the grid and updates the items.
// Parameters:
// int width - The new grid width.
//=============================================================================
void UIGrid::setWidth(int width)
{
	if (m_rect)
	{
		if (m_background)
		{
			m_background->setWidth(width);
		}

		m_rect->setWidth(width);

		int itemWidth = m_rect->width() / m_itemsPerRow;

		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			m_items[i]->setWidth(itemWidth);
		}

		updatePositions();
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the grid and adjusts the items.
// Parameters:
// int height - The new grid height.
//=============================================================================
void UIGrid::setHeight(int height)
{
	if (m_rect)
	{
		m_rect->setHeight(height);

		if (m_background)
		{
			m_background->setHeight(height);
		}

		int itemHeight = m_rect->height() / m_rowCount;

		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			m_items[i]->setHeight(itemHeight);
		} 

		updatePositions();
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state of the grid and its items.
// Parameters:
// bool active - The new active state.
//=============================================================================
void UIGrid::setActive(bool active)
{
	if (m_active != active)
	{
		m_active = active;

		if (m_background)
		{
			m_background->setActive(m_active);
		}

		UIComponent *currentItem = getCurrentItem();

		if (currentItem)
		{
			if (currentItem->getType() == UIComponent::UI_BUTTON)
			{
				UIButton *button =
					static_cast<UIButton*>(currentItem);

				button->setActive(active);
			}
		}
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visible state of the grid and its items.
// Parameters:
// bool visible - The visible state to set.
//=============================================================================
void UIGrid::setVisible(bool visible)
{
	if (m_visible != visible)
	{
		m_visible = visible;

		if (m_background)
		{
			m_background->setVisible(m_visible);
		}

		int startingItem = m_rowOffset * m_itemsPerRow;
		int endingItem = startingItem + (m_rowCount - 1) * m_itemsPerRow;

		if (m_items.size() <= endingItem)
		{
			endingItem = m_items.size() - 1;
		}

		for (unsigned int i = startingItem; i <= endingItem; i++)
		{
			m_items[i]->setVisible(m_visible);
		}
	}
}

//=============================================================================
// Function: void addItem(UIComponent*)
// Description:
// Adds an item to the grid.
// Parameters:
// UIComponent *item - The item to add.
//=============================================================================
void UIGrid::addItem(UIComponent *item)
{
	if (item)
	{
		item->setWidth(m_rect->width() / m_itemsPerRow);
		item->setHeight(m_rect->height() / m_rowCount);

		m_items.push_back(item);

		updatePositions();
	}
}

//=============================================================================
// Function: void removeItem(int)
// Description: 
// Finds the item with the specified ID and removes it.
// Parameters:
// int entityID - The ID of the item to remove.
//=============================================================================
void UIGrid::removeItem(int entityID)
{
	if (m_background)
	{
		if (m_background->getEntityID() == entityID)
		{
			delete m_background;
			m_background = NULL;

			return;
		}
	}

	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i]->getEntityID() == entityID)
		{
			delete m_items[i];
			m_items[i] = NULL;

			auto vit = m_items.begin();
			std::advance(vit, i);

			vit = m_items.erase(vit);

			break;
		}
	}
}