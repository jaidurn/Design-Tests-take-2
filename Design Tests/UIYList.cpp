#include "UIYList.h"
#include "UIGraphic.h"
#include "UIButton.h"
#include "UIText.h"

#include "MessageSystem.h"
#include "InputMessage.h"
#include "EntityDestroyMessage.h"

#include "InputDevice.h"

#include <iostream>

UIYList::UIYList(int entityID,
	string name,
	Vector2D position,
	int width,
	int height,
	int itemsVisible)
	:UIGroup(entityID, name, position, width, height),
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

//=============================================================================
// Function: void removeItem(int)
// Description:
// Finds and removes the item with the specified ID.
// Parameters:
// int entityID - The entity ID of the item to remove.
//=============================================================================
void UIYList::removeItem(int entityID)
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
		if (m_items[i])
		{
			if (m_items[i]->getEntityID() == entityID)
			{
				m_items[i] = NULL;

				// Erase the spot and move everything.
				auto vit = m_items.begin();
				std::advance(vit, i);

				vit = m_items.erase(vit);

				break;
			}
		}
	}

	updatePositions();
}

//=============================================================================
// Function: void removeItem(string)
// Description:
// Finds and removes the item with the specified name.
// Parameters:
// string name - The name of the item.
//=============================================================================
void UIYList::removeItem(string name)
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
		if (m_items[i])
		{
			if (m_items[i]->getName() == name)
			{
				m_items[i] = NULL;

				auto vit = m_items.begin();
				std::advance(vit, i);

				vit = m_items.erase(vit);

				break;
			}
		}
	}
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes messages passed to the vertical list and calls the
// needed functions.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIYList::processMessage(IMessage *message)
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
// Function: void update()
// Description:
// Updates the vertical list and all of its active items.
//=============================================================================
void UIYList::update()
{
	if (m_active)
	{
		UIComponent *currentItem = getCurrentItem();

		if (currentItem)
		{
			if (currentItem->getType() == UIComponent::UI_BUTTON)
			{
				UIButton *button =
					static_cast<UIButton*>(currentItem);

				button->update();
			}
		}
	}
}

//=============================================================================
// Function: UIComponent* getCurrentItem()
// Description:
// Gets the current item.
// Output:
// UIComponent*
// On success - Returns a pointer to the current item in the list.
// On failure - Returns NULL.	
//=============================================================================
UIComponent* UIYList::getCurrentItem()
{
	UIComponent *current = NULL;

	if (m_currentItem < (int)m_items.size())
	{
		current = m_items[m_currentItem];
	}

	return current;
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Deletes all of the items and frees the memory.
//=============================================================================
void UIYList::cleanUp()
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
// Updates the positions of all of the items.
//=============================================================================
void UIYList::updatePositions()
{
	if (m_rect)
	{
		int itemHeight = m_rect->height() / m_itemsVisible;
		int yOffset = itemHeight * -m_itemOffset;
		float itemX = m_rect->center().getX();
		float startingY = m_rect->getTopLeft().getY() + (float)(itemHeight / 2) + yOffset;

		float topVisibleY = m_rect->getTopLeft().getY() + (float)(itemHeight / 2);
		float bottomVisibleY = m_rect->getBottomLeft().getY() - (float)(itemHeight / 2);

		float currentY = startingY;

		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			m_items[i]->setPosition(Vector2D(itemX, currentY));

			if (topVisibleY <= currentY &&
				currentY <= bottomVisibleY)
			{
				m_items[i]->setVisible(true);
			}
			else
			{
				m_items[i]->setVisible(false);
			}

			if (i == m_currentItem)
			{
				m_items[i]->setActive(true);
			}
			else
			{
				m_items[i]->setActive(false);
			}

			currentY += itemHeight;
		}
	}
}

//=============================================================================
// Function: void adjustCurrentItem(int)
// Description:
// Moves the current item by the specified amount.
// Parameters:
// int amount - The amount to move the current item by.
//=============================================================================
void UIYList::adjustCurrentItem(int amount)
{
	int newItem = m_currentItem + amount;

	// Make sure we're in bounds.
	if (newItem < 0)
	{
		newItem = 0;
	}
	else if ((int)m_items.size() <= newItem)
	{
		newItem = (int)m_items.size() - 1;
	}

	if (newItem < m_itemOffset)
	{
		m_itemOffset = newItem;
	}
	else if (m_itemOffset + m_itemsVisible - 1 < newItem)
	{
		m_itemOffset = newItem - (m_itemsVisible - 1);
	}

	m_currentItem = newItem;

	updatePositions();
}

//=============================================================================
// Function: void processInputMsg(InputMessage*)
// Description:
// Processes Input Messages for the vertical list and items.
// Parameters:
// InputMesage *inputMsg - The input message to process.
//=============================================================================
void UIYList::processInputMsg(InputMessage *inputMsg)
{
	if (inputMsg && m_active)
	{
		switch (inputMsg->m_inputType)
		{
		case InputMessage::INPUT_BUTTON:
		{
			Uint32 ticksPassed = SDL_GetTicks() - m_prevUpdate;

			if (m_UPDATE_COOLDOWN <= ticksPassed)
			{
				InputButtonMessage *button =
					static_cast<InputButtonMessage*>(inputMsg);

				bool listUpdated = false;

				switch (button->m_buttonCode)
				{
				case InputDevice::ANALOG_UP:
				{
					adjustCurrentItem(-1);

					listUpdated = true;
					break;
				}
				case InputDevice::ANALOG_RIGHT:
				{
					int itemOffset =
						(m_itemOffset + m_itemsVisible - 1) - m_currentItem;

					if (itemOffset == 0)
					{
						itemOffset = 1;
					}

					adjustCurrentItem(itemOffset);

					listUpdated = true;
					break;
				}
				case InputDevice::ANALOG_DOWN:
				{
					adjustCurrentItem(1);

					listUpdated = true;
					break;
				}
				case InputDevice::ANALOG_LEFT:
				{
					int itemOffset =
						m_itemOffset - m_currentItem;

					if (itemOffset == 0)
					{
						itemOffset = -1;
					}

					adjustCurrentItem(itemOffset);

					listUpdated = true;
					break;
				}
				}

				if (listUpdated)
				{
					m_prevUpdate = SDL_GetTicks();
				}
			}

			break;
		}
		case InputMessage::INPUT_MOVE:
		{
			break;
		}
		case InputMessage::INPUT_AXIS:
		{
			Uint32 ticksPassed = SDL_GetTicks() - m_prevUpdate;

			if (m_UPDATE_COOLDOWN <= ticksPassed)
			{
				bool listUpdated = false;

				InputAxisMessage *axisMsg =
					static_cast<InputAxisMessage*>(inputMsg);

				if (axisMsg->m_axis == SDL_CONTROLLER_AXIS_LEFTY)
				{
					if (axisMsg->m_axisMovement < 0)
					{
						listUpdated = true;
						adjustCurrentItem(-1);
					}
					else if (0 < axisMsg->m_axisMovement)
					{
						listUpdated = true;
						adjustCurrentItem(1);
					}
				}

				if (listUpdated)
				{
					m_prevUpdate = SDL_GetTicks();
				}
			}

			break;
		}
		}
	}
}

//=============================================================================
// Function: void processEntityDestroyMsg(EntityDestroyMessage*)
// Description:
// Processes the provided Entity Destroy Message and removes any
// items related to it.
// Parameters:
// EntityDestroyMessage *destroyMsg - The destroy message to process.
//=============================================================================
void UIYList::processEntityDestroyMsg(EntityDestroyMessage *destroyMsg)
{
	if (destroyMsg)
	{
		removeItem(destroyMsg->m_entityID);
	}
}