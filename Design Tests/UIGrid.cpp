#include "UIGrid.h"

#include "UIGraphic.h"
#include "UIButton.h"

#include "MessageSystem.h"
#include "InputMessage.h"
#include "EntityDestroyMessage.h"

#include "InputDevice.h"

UIGrid::UIGrid(int entityID,
	string name,
	Vector2D position,
	int width,
	int height,
	int itemsPerRow,
	int rowCount)
	:UIGroup(entityID, name, position, width, height),
	m_itemsPerRow(itemsPerRow),
	m_rowCount(rowCount),
	m_rowOffset(0),
	m_currentItem(0),
	m_prevUpdate(0)
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

		if ((int)m_items.size() <= endingItem)
		{
			endingItem = m_items.size() - 1;
		}

		for (int i = startingItem; i <= endingItem; i++)
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

//=============================================================================
// Function: void removeItem(string)
// Description:
// Finds the item with the specified name and removes it.
// Parameters:
// string name - The name of the item.
//=============================================================================
void UIGrid::removeItem(string name)
{
	if (m_background)
	{
		if (m_background->getName() == name)
		{
			delete m_background;
			m_background = NULL;

			return;
		}
	}

	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		if(m_items[i]->getName() == name)
		{
			m_items[i] = NULL;

			auto vit = m_items.begin();
			std::advance(vit, i); 
			
			vit = m_items.erase(vit);

			break;
		}
	}
}

//=============================================================================
// Function: UIComponent* getCurrentItem()
// Description:
// Gets the current item if there is one.
// Output:
// UIComponent*
// On success - Returns a pointer to the current item.
// On failure - Returns NULL.
//=============================================================================
UIComponent* UIGrid::getCurrentItem()
{
	UIComponent *item = NULL;

	if (m_currentItem < (int)m_items.size())
	{
		item = m_items[m_currentItem];
	}

	return item;
}

//=============================================================================
// Function: void update()
// Description:
// Updates the grid and all of its items.
//=============================================================================
void UIGrid::update()
{
	UIComponent *current = getCurrentItem();

	if (current)
	{
		if (current->getType() == UIComponent::UI_BUTTON)
		{
			UIButton *button =
				static_cast<UIButton*>(current);

			button->update();
		}
	}
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes messages and decides whether or not to handle them.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIGrid::processMessage(IMessage *message)
{
	if (message)
	{
		switch (message->type())
		{
		case IMessage::INPUT:
		{
			InputMessage *inputMsg =
				static_cast<InputMessage*>(message);

			processInputMsg(inputMsg);

			break;
		}
		case IMessage::ENTITY_DESTROY:
		{
			EntityDestroyMessage *destroyMsg =
				static_cast<EntityDestroyMessage*>(message);

			processEntityDestroyMsg(destroyMsg);

			break;
		}
		}
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up all of the items and frees the memory.
//=============================================================================
void UIGrid::cleanUp()
{
	if (m_background)
	{
		delete m_background;
		m_background = NULL;
	}

	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		delete m_items[i];
		m_items[i] = NULL;
	}

	m_items.clear();
}

//=============================================================================
// Function: void updatePositions()
// Description:
// Updates all of the item positions and sets them to visible
// as needed.
//=============================================================================
void UIGrid::updatePositions()
{
	int itemWidth = m_rect->width() / m_itemsPerRow;
	int itemHeight = m_rect->height() / m_rowCount;
	int yOffset = itemHeight * -m_rowOffset;
	int startingX = (int)m_rect->getTopLeft().getX() + (itemWidth / 2);
	int startingY = (int)m_rect->getTopLeft().getY() + yOffset + (itemHeight / 2);

	int currentX = startingX;
	int currentY = startingY;

	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		m_items[i]->setPosition(Vector2D((float)currentX, (float)currentY));

		if ((int)m_rect->getTopLeft().getY() < currentY &&
			currentY < (int)m_rect->getBottomLeft().getY())
		{
			m_items[i]->setVisible(m_visible);
		}
		else
		{
			m_items[i]->setVisible(false);
		}

		currentX += itemWidth;

		if (m_rect->getTopRight().getX() <= currentX)
		{
			currentX = startingX;
			currentY += itemHeight;
		}
	}
}

//=============================================================================
// Function: void adjustCurrentItem(int)
// Description:
// Adjusts the current item by the specified amount.
// Positive means forward, negative means backward.
// Parameters:
// int amount - The amount to adjust the current item by.
//=============================================================================
void UIGrid::adjustCurrentItem(int amount)
{
	int newCurrent = m_currentItem + amount;

	if (newCurrent < 0)
	{
		newCurrent = 0;
	}
	else if ((int)m_items.size() <= newCurrent)
	{
		newCurrent = (int)m_items.size() - 1;
	}

	int currentRow = newCurrent / m_itemsPerRow;

	if (currentRow < m_rowOffset)
	{
		m_rowOffset = currentRow;
	}
	else if (m_rowOffset + m_rowCount - 1 < currentRow)
	{
		m_rowOffset = currentRow - (m_rowCount - 1);
	}

	m_currentItem = newCurrent;

	updatePositions();
}

//=============================================================================
// Function: void processInputMsg(InputMessage*)
// Description:
// Processes input messages and handles button presses.
// Parameters:
// InputMessage *inputMsg - The input message to process.
//=============================================================================
void UIGrid::processInputMsg(InputMessage *inputMsg)
{
	if (inputMsg)
	{
		Uint32 passedTicks = SDL_GetTicks() - m_prevUpdate;

		if (m_UPDATE_COOLDOWN <= passedTicks)
		{
			bool updateTicks = false;

			switch (inputMsg->m_inputType)
			{
			case InputMessage::INPUT_BUTTON:
			{
				InputButtonMessage *button =
					static_cast<InputButtonMessage*>(inputMsg);

				if (button->m_pressed)
				{
					switch (button->m_buttonCode)
					{
					case InputDevice::ANALOG_UP:
					{
						adjustCurrentItem(-m_itemsPerRow);

						updateTicks = true;
						break;
					}
					case InputDevice::ANALOG_RIGHT:
					{
						adjustCurrentItem(1);

						updateTicks = true;
						break;
					}
					case InputDevice::ANALOG_DOWN:
					{
						adjustCurrentItem(m_itemsPerRow);
						
						updateTicks = true;
						break;
					}
					case InputDevice::ANALOG_LEFT:
					{
						adjustCurrentItem(-1);

						updateTicks = true;
						break;
					}
					case InputDevice::SELECT:
					{
						break;
					}
					}
				}

				break;
			}
			case InputMessage::INPUT_AXIS:
			{
				InputAxisMessage *axis =
					static_cast<InputAxisMessage*>(inputMsg);

				if (axis->m_axis == SDL_CONTROLLER_AXIS_LEFTX)
				{
					if (axis->m_axisMovement < 0)
					{
						adjustCurrentItem(-1);
						updateTicks = true;
					}
					else if (0 < axis->m_axisMovement)
					{
						adjustCurrentItem(1);
						updateTicks = true;
					}
				}
				else if (axis->m_axis == SDL_CONTROLLER_AXIS_LEFTY)
				{
					if (axis->m_axisMovement < 0)
					{
						adjustCurrentItem(-m_itemsPerRow);
						updateTicks = true;
					}
					else if (0 < axis->m_axisMovement)
					{
						adjustCurrentItem(m_itemsPerRow);
						updateTicks = true;
					}
				}

				break;
			}
			}

			if (updateTicks)
			{
				m_prevUpdate = SDL_GetTicks();
			}
		}
	}
}

//=============================================================================
// Function: void processEntityDestroyMsg(EntityDestroyMessage*)
// Description:
// Processes the entity destroy message and removes any items
// related to the destroyed ID.
// Parameters:
// EntityDestroyMessage *destroyMsg - The destroy message to process.
//=============================================================================
void UIGrid::processEntityDestroyMsg(EntityDestroyMessage *destroyMsg)
{
	if (destroyMsg)
	{
		removeItem(destroyMsg->m_entityID);
	}
}