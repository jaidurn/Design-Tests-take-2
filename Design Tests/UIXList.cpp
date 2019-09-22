#include "UIXList.h"
#include "MessageSystem.h"
#include "InputMessage.h"
#include "EntityDestroyMessage.h"
#include "UIGraphic.h"
#include "UIButton.h"

#define downArrow SDLK_DOWN
#define upArrow SDLK_UP
#define leftArrow SDLK_LEFT
#define rightArrow SDLK_RIGHT
#define leftMouseButton SDL_BUTTON_LEFT
#define selectButton SDL_CONTROLLER_BUTTON_A

UIXList::UIXList(int entityID, 
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
	m_prevUpdate = SDL_GetTicks();
}

UIXList::~UIXList()
{
	cleanUp();
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the center position of the list and moves the items in it.
// Parameters:
// Vector2D position - The new center position of the list.
//=============================================================================
void UIXList::setPosition(Vector2D position)
{
	if (m_rect)
	{
		if (m_background)
		{
			m_background->setPosition(position);
		}

		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			Vector2D offset = m_items[i]->getPosition() - m_rect->center();

			m_items[i]->setPosition(offset + position);
		}

		m_rect->setCenter(position);
	}
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the list and adjusts the widths of the items in it.
// Parameters:
// int width - The new width of the list.
//=============================================================================
void UIXList::setWidth(int width)
{
	if (m_rect)
	{
		if (m_background)
		{
			m_background->setWidth(width);
		}

		int widthDiff = width - m_rect->width();

		if (widthDiff != 0)
		{
			if (widthDiff < 0) { widthDiff *= -1; }

			for (unsigned int i = 0; i < m_items.size(); i++)
			{
				Vector2D itemPos = m_items[i]->getPosition();
				int xFix = widthDiff / 2;

				if (m_rect->center().getX() < itemPos.getX())
				{
					xFix *= -1;
				}
				// Force it to round, just in case they aren't already
				else if ((int)m_rect->center().getX() == (int)itemPos.getX())
				{
					xFix = 0;
				}

				itemPos.setX(itemPos.getX() + (float)xFix);

				m_items[i]->setPosition(itemPos);
				m_items[i]->setWidth(width / m_itemsVisible);
			}

			m_rect->setWidth(width);
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the list and all of the list items.
// Parameters:
// int height - The new height of the list.
//=============================================================================
void UIXList::setHeight(int height)
{
	if (m_rect)
	{
		if (m_background)
		{
			m_background->setHeight(height);
		}

		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			m_items[i]->setHeight(height);
		}

		m_rect->setHeight(height);
	}
}

//=============================================================================
// Function: void addItem(UIComponent*)
// Description:
// Adds an item to the list and adjusts it to it's position.
// Parameters:
// UIComponent *item - The item to add to the list.
//=============================================================================
void UIXList::addItem(UIComponent *item)
{
	if (item)
	{
		item->setWidth(m_rect->width() / m_itemsVisible);
		item->setHeight(m_rect->height());

		int currentItem = (int)m_items.size();
		int itemWidth = m_rect->width() / m_itemsVisible;

		int xOffset = (int)m_rect->getTopLeft().getX() + (itemWidth * (currentItem - m_itemOffset) + (itemWidth / 2));

		Vector2D itemPos((float)xOffset, m_rect->center().getY());

		item->setPosition(itemPos);

		if (currentItem < m_itemOffset ||
			m_itemOffset + (m_itemsVisible - 1) < currentItem)
		{
			item->setVisible(false);
		}

		item->setActive(false);

		if (currentItem == m_currentItem)
		{
			if (item->getType() == UIComponent::UI_BUTTON)
			{
				UIButton *button =
					static_cast<UIButton*>(item);

				button->setText("THIS");
			}
		}

		m_items.emplace_back(item);
	}
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Handles the messages passed to it.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIXList::processMessage(IMessage *message)
{
	if (message)
	{
		switch (message->type())
		{
		case IMessage::INPUT:
		{
			InputMessage *inputMsg = static_cast<InputMessage*>(message);

			processInput(inputMsg);

			break;
		}
		case IMessage::ENTITY_DESTROY:
		{
			EntityDestroyMessage *destroyMsg = static_cast<EntityDestroyMessage*>(message);

			processEntityDestroy(destroyMsg);
			
			break;
		}
		}

		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			if (m_items[i]->getType())
			{
				UIButton *button = static_cast<UIButton*>(m_items[i]);

				button->processMessage(message);
			}
		}
	}
}

//=============================================================================
// Function: void update()
// Description:
// Updates the list and all of the items in it.
//=============================================================================
void UIXList::update()
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
// Function: UIComponent* getCurrentItem()
// Description:
// Gets the current selected item.
// Output:
// UIComponent*
// On success - Returns a pointer to the selected component.
// On failure - Returns NULL.
//=============================================================================
UIComponent* UIXList::getCurrentItem()
{
	UIComponent* current = NULL;

	if (m_currentItem < (int)m_items.size())
	{
		current = m_items[m_currentItem];
	}

	return current;
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Deletes all of the items and frees up the ram.
//=============================================================================
void UIXList::cleanUp()
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

	if (m_rect)
	{
		delete m_rect;
		m_rect = NULL;
	}
}

//=============================================================================
// Function: void updatePositions()
// Description:
// Updates the positions of all of the items based on the
// offset.
//=============================================================================
void UIXList::updatePositions()
{
	int itemWidth = m_rect->width() / m_itemsVisible;
	int startingX = (int)m_rect->getTopLeft().getX() + (itemWidth * -m_itemOffset) + (itemWidth / 2);
	int currentX = startingX;

	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		Vector2D itemPos = m_items[i]->getPosition();

		itemPos.setX((float)currentX);

		m_items[i]->setPosition(itemPos);

		if (i < m_itemOffset ||
			m_itemOffset + (m_itemsVisible - 1) < i)
		{
			m_items[i]->setVisible(false);
			m_items[i]->setActive(false);
		}
		else if(i != m_currentItem)
		{
			m_items[i]->setActive(false);
			m_items[i]->setVisible(true);
		}
		else
		{
			m_items[i]->setActive(true);
			m_items[i]->setVisible(true);
		}

		currentX += itemWidth;
	}
}

//=============================================================================
// Function: void adjustCurrentItem(int)
// Description:
// Adjusts the index of the current item by the specified amount.
// Parameters:
// int amount - The amount to move the currentItem index by.
//=============================================================================
void UIXList::adjustCurrentItem(int amount)
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

	if (newCurrent < m_itemOffset)
	{
		m_itemOffset = newCurrent;
	}
	else if (m_itemOffset + (m_itemsVisible - 1) < newCurrent)
	{
		m_itemOffset = newCurrent - (m_itemsVisible - 1);
	}

	if (m_items[m_currentItem]->getType() == UIComponent::UI_BUTTON)
	{
		UIButton *button =
			static_cast<UIButton*>(m_items[m_currentItem]);

		button->setText("Item " + std::to_string(m_currentItem));
	}

	m_currentItem = newCurrent;

	if (m_items[m_currentItem]->getType() == UIComponent::UI_BUTTON)
	{
		UIButton *button =
			static_cast<UIButton*>(m_items[m_currentItem]);

		button->setText("THIS");
	}

	updatePositions();
}

//=============================================================================
// Function: void processInput(InputMessage*)
// Description:
// Processes the input message and passes it to its items.
// Parameters:
// InputMessage *inputMsg - The input message to process.
//=============================================================================
void UIXList::processInput(InputMessage *inputMsg)
{
	if (m_active)
	{
		if (inputMsg)
		{
			switch (inputMsg->m_inputType)
			{
			case InputMessage::INPUT_BUTTON:
			{
				InputButtonMessage *button = static_cast<InputButtonMessage*>(inputMsg);

				Uint32 timeElipsed = SDL_GetTicks() - m_prevUpdate;

				if (m_UPDATE_COOLDOWN <= timeElipsed)
				{
					if (button->m_pressed)
					{
						switch (button->m_buttonCode)
						{
						case InputDevice::ANALOG_UP:
						{
							int offsetDifference = m_currentItem - m_itemOffset;

							if (offsetDifference == 0)
							{
								offsetDifference = 1;
							}

							adjustCurrentItem(-offsetDifference);

							m_prevUpdate = SDL_GetTicks();

							break;
						}
						case InputDevice::ANALOG_LEFT:
						{
							adjustCurrentItem(-1);

							m_prevUpdate = SDL_GetTicks();

							break;
						}
						case InputDevice::ANALOG_DOWN:
						{
							int offsetDifference =
								(m_itemOffset + m_itemsVisible - 1) - m_currentItem;

							if (offsetDifference == 0)
							{
								offsetDifference = 1;
							}

							adjustCurrentItem(offsetDifference);

							m_prevUpdate = SDL_GetTicks();

							break;
						}
						case InputDevice::ANALOG_RIGHT:
						{
							adjustCurrentItem(1);

							m_prevUpdate = SDL_GetTicks();

							break;
						}
						}
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
				InputAxisMessage *axisMsg =
					static_cast<InputAxisMessage*>(inputMsg);

				Uint32 timeElipsed = SDL_GetTicks() - m_prevUpdate;

				if (m_UPDATE_COOLDOWN <= timeElipsed)
				{
					if (axisMsg->m_axis == SDL_CONTROLLER_AXIS_LEFTX)
					{
						if (axisMsg->m_axisMovement < 0)
						{
							adjustCurrentItem(-1);

							m_prevUpdate = SDL_GetTicks();
						}
						else if (0 < axisMsg->m_axisMovement)
						{
							adjustCurrentItem(1);

							m_prevUpdate = SDL_GetTicks();
						}
					}
				}

				break;
			}
			}
		}
	}
}

void UIXList::processEntityDestroy(EntityDestroyMessage *destroyMsg)
{
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i]->getEntityID() == destroyMsg->m_entityID)
		{
			if (m_items[i])
			{
				delete m_items[i];
			}

			m_items[i] = NULL;
		}
	}
}