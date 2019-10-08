#include "UIMenu.h"
#include "UIGraphic.h"
#include "UIButton.h"
#include "UIText.h"

#include "MessageSystem.h"
#include "InputMessage.h"
#include "CameraMoveMessage.h"
#include "EntityDestroyMessage.h"

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "KeyboardMouse.h"
#include "InputSystem.h"

UIMenu::UIMenu(int entityID,
	string name,
	Vector2D position,
	int width,
	int height,
	int itemsWide,
	int itemsHigh)
	:m_rect(NULL),
	m_background(NULL),
	m_itemsWide(itemsWide),
	m_itemsHigh(itemsHigh),
	m_currentX(0),
	m_currentY(0),
	m_prevUpdate(0),
	m_docked(true),
	m_visible(true),
	m_active(true)
{
	if (m_itemsWide <= 0)
	{
		m_itemsWide = 1;
	}

	if (m_itemsHigh <= 0)
	{
		m_itemsHigh = 1;
	}

	m_travelMap.resize(m_itemsWide);

	for (unsigned int i = 0; i < m_travelMap.size(); i++)
	{
		for (int j = 0; j < m_itemsHigh; j++)
		{
			m_travelMap[i].push_back(NULL);
		}
	}

	m_rect = new Rectangle(position, width, height);
}

UIMenu::~UIMenu()
{
	cleanUp();
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the center position of the menu and updates its groups.
// Parameters:
// Vector2D position - The new center position.
//=============================================================================
void UIMenu::setPosition(Vector2D position)
{
	if (m_rect)
	{
		if (m_background)
		{
			m_background->setPosition(position);
		}

		auto groupIt = m_groups.begin();

		while (groupIt != m_groups.end())
		{
			if (groupIt->second)
			{
				UIGroup *group = groupIt->second;

				Vector2D offset =
					group->getPosition() - m_rect->center();

				group->setPosition(position + offset);
			}

			groupIt++;
		}

		m_rect->setCenter(position);
	}
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the menu width.
// TODO: Decide if this scales the group items or not.
// Parameters:
// int width - The new width of the menu.
//=============================================================================
void UIMenu::setWidth(int width)
{
	if (m_rect)
	{
		m_rect->setWidth(width);

		if (m_background)
		{
			m_background->setWidth(m_rect->width());
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the menu.
// TODO: Decide if this scales the group items or not.
// Parameters:
// int height - The new height of the menu.
//=============================================================================
void UIMenu::setHeight(int height)
{
	if (m_rect)
	{
		m_rect->setHeight(height);

		if (m_background)
		{
			m_background->setHeight(m_rect->height());
		}
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visible state of the menu and its groups.
// Parameters:
// bool visible - The new visible state.
//=============================================================================
void UIMenu::setVisible(bool visible)
{
	if (m_visible != visible)
	{
		m_visible = visible;

		auto groupIt = m_groups.begin();

		while (groupIt != m_groups.end())
		{
			if (groupIt->second)
			{
				UIGroup *group = groupIt->second;

				group->setVisible(m_visible);
			}

			groupIt++;
		}

		if (m_background)
		{
			m_background->setVisible(m_visible);
		}
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state of the menu and its groups.
// Parameters:
// bool active - The new active state.
//=============================================================================
void UIMenu::setActive(bool active)
{
	if (m_active != active)
	{
		m_active = active;

		if (!m_active)
		{
			auto groupIt = m_groups.begin();

			while (groupIt != m_groups.end())
			{
				if (groupIt->second)
				{
					UIGroup *group = groupIt->second;

					group->setActive(m_active);
				}

				groupIt++;
			}
		}

		if (m_background)
		{
			m_background->setActive(m_active);
		}
	}
}

//=============================================================================
// Function: void setDocked(bool)
// Description:
// Sets the docked state of the menu.
// Parameters:
// bool dockable - The new docked state.
//=============================================================================
void UIMenu::setDocked(bool docked)
{
	m_docked = docked;
}

//=============================================================================
// Function: void setBackground(UIGraphic*)
// Description:
// Sets the background of the menu and makes sure its the right size.
// Parameters:
// UIGraphic *background - The new background to set.
//=============================================================================
void UIMenu::setBackground(UIGraphic *background)
{
	if (m_background)
	{
		delete m_background;
	}

	m_background = background;

	m_background->setWidth(m_rect->width());
	m_background->setHeight(m_rect->height());
	m_background->setPosition(m_rect->center());
}

//=============================================================================
// Function: void addGroup(UIGroup*)
// Description:
// Adds the group to the menu. This adds it to the map and finds its
// place on the travel map.
// Parameters:
// UIGroup *group - The new group to add.
//=============================================================================
void UIMenu::addGroup(UIGroup *group)
{
	if (group)
	{
		// Make sure it's not in the menu already.
		if (!getGroup(group->getEntityID()))
		{
			int itemWidth = m_rect->width() / m_itemsWide;
			int itemHeight = m_rect->height() / m_itemsHigh;
			
			Vector2D groupPos = group->getPosition();

			int groupTravelX = (int)groupPos.getX() / itemWidth;
			int groupTravelY = (int)groupPos.getY() / itemHeight;
			
			// Insert the group into our travel map
			insertGroup(group, groupTravelX, groupTravelY);

			group->setPosition(group->getPosition() + m_rect->getTopLeft());
			group->setActive(false);
			group->setVisible(m_visible);

			m_groups.insert(std::make_pair(group->getEntityID(), group));
		}
	}
}

//=============================================================================
// Function: void removeGroup(int entityID)
// Description:
// Finds and removes the group with the specified entity ID.
// Parameters:
// int entityID - The entity ID of the group.
//=============================================================================
void UIMenu::removeGroup(int entityID)
{
	auto groupIt = m_groups.begin();

	while (groupIt != m_groups.end())
	{
		if (groupIt->second)
		{
			UIGroup *group = groupIt->second;

			if (group->getEntityID() == entityID)
			{
				removeFromTravelMap(entityID);
				
				delete groupIt->second;
				groupIt = m_groups.erase(groupIt);
			}
		}

		groupIt++;
	}
}

//=============================================================================
// Function: void removeGroup(string)
// Description:
// Finds and removes the group with the specified name.
// Parameters:
// string name - The name of the group.
//=============================================================================
void UIMenu::removeGroup(string name)
{
	auto groupIt = m_groups.begin();

	while (groupIt != m_groups.end())
	{
		if (groupIt->second)
		{
			UIGroup *group = groupIt->second;

			if (group->getName() == name)
			{
				removeFromTravelMap(group->getEntityID());

				delete groupIt->second;

				groupIt = m_groups.erase(groupIt);
			}
		}

		groupIt++;
	}
}

//=============================================================================
// Function: UIGroup* getGroup(int) const
// Description:
// Finds and returns a the group with the specified ID.
// Parameters:
// int entityID - The ID of the group to find.
// Output:
// UIGroup*
// On success - Returns a pointer to the found group.
// On failure - Returns NULL.
//=============================================================================
UIGroup* UIMenu::getGroup(int entityID) const
{
	UIGroup *group = NULL;

	auto groupIt = m_groups.find(entityID);

	if (groupIt != m_groups.end())
	{
		group = groupIt->second;
	}

	return group;
}

//=============================================================================
// Function: UIGroup* getGroup(string) const
// Description:
// Finds and returns the group with the specified name.
// Parameters:
// string name - The name of the group to find.
// Output:
// UIGroup*
// On success - Returns a pointer to the found group.
// On failure - Returns NULL.
//=============================================================================
UIGroup* UIMenu::getGroup(string name) const
{
	UIGroup *group = NULL;

	auto groupIt = m_groups.begin();

	while (groupIt != m_groups.end() && !group)
	{
		if (groupIt->second->getName() == name)
		{
			group = groupIt->second;
		}

		groupIt++;
	}

	return group;
}

//=============================================================================
// Function: UIGroup* getCurrentGroup() const
// Description:
// Gets the currently selected group.
// Output:
// UIGroup*
// On success - Returns a pointer to the current group.
// On failure - Returns NULL.
//=============================================================================
UIGroup* UIMenu::getCurrentGroup() const
{
	UIGroup *group = NULL;

	group = m_travelMap[m_currentX][m_currentY];

	return group;
}

//=============================================================================
// Function: void update()
// Description:
// Updates the menu and the current group.
//=============================================================================
void UIMenu::update()
{
	if (m_active)
	{
		UIGroup *current = getCurrentGroup();

		if (current)
		{
			current->update();
		}
	}
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes messages and calls the right functions for them.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIMenu::processMessage(IMessage *message)
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
		case IMessage::CAMERA_MOVE:
		{
			if (m_docked)
			{
				CameraMoveMessage *moveMsg =
					static_cast<CameraMoveMessage*>(message);

				processCameraMoveMsg(moveMsg);
			}

			break;
		}
		}
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up the groups and frees memory.
//=============================================================================
void UIMenu::cleanUp()
{
	auto groupIt = m_groups.begin();

	while (groupIt != m_groups.end())
	{
		delete groupIt->second;
		groupIt = m_groups.erase(groupIt);

		if (groupIt != m_groups.end())
		{
			groupIt++;
		}
	}

	if (m_background)
	{
		delete m_background;
		m_background = NULL;
	}

	m_travelMap.clear();
}

//=============================================================================
// Function: void insertGroup(UIGroup*, int, int)
// Description:
// Inserts a group into the travel map.
// Parameters:
// UIGroup *group - The group to add to the travel map.
// int mapX - The X position to try to move to.
// int mapY - The y position to try to move to.
//=============================================================================
void UIMenu::insertGroup(UIGroup* group, int mapX, int mapY)
{
	if (group)
	{
		if (mapX < 0)
		{
			mapX = 0;
		}

		if (mapY < 0)
		{
			mapY = 0;
		}

		int currentX = mapX;
		int currentY = mapY;

		bool placed = false;

		while (!placed)
		{
			if (m_itemsWide <= currentX)
			{
				int difference = currentX - m_itemsWide;

				m_travelMap.resize(currentX);

				for (int i = m_itemsWide - 1; i < currentX; i++)
				{
					for (int j = 0; j < m_itemsHigh; j++)
					{
						m_travelMap[i].push_back(NULL);
					}
				}
			}

			if (m_itemsHigh <= currentY)
			{
				int difference = currentY - m_itemsHigh;

				for (unsigned int i = 0; i < m_travelMap.size(); i++)
				{
					for (int j = 0; j <= difference; j++)
					{
						m_travelMap[i].push_back(NULL);
					}
				}
			}

			UIGroup *existingGroup = m_travelMap[currentX][currentY];

			if (existingGroup)
			{
				Vector2D existingPos = existingGroup->getPosition();
				Vector2D groupPos = group->getPosition();

				if (groupPos.getX() < existingPos.getX())
				{
					if (0 < currentX)
					{
						currentX -= 1;
					}
					else
					{
						m_travelMap[currentX][currentY] = group;

						placed = true;

						insertGroup(existingGroup, currentX + 1, currentY);
					}
				}
				else if (existingPos.getX() <= groupPos.getX())
				{
					currentX += 1;
				}
				else if (groupPos.getY() < existingPos.getY())
				{
					if (0 < currentY)
					{
						currentY -= 1;
					}
					else
					{
						m_travelMap[currentX][currentY] = group;

						placed = true;

						insertGroup(existingGroup, currentX, currentY + 1);
					}
				}
				else if (existingPos.getY() <= groupPos.getY())
				{
					currentY += 1;
				}
			}
			else
			{
				m_travelMap[currentX][currentY] = group;
				placed = true;
			}
		}

		if (!getCurrentGroup())
		{
			m_currentX = currentX;
			m_currentY = currentY;
		}
	}
}

//=============================================================================
// Function: void removeFromTravelMap(int)
// Description:
// Finds and removes the group with the specified ID from the travel map.
// Parameters:
// int entityID - The ID of the group to remove.
//=============================================================================
void UIMenu::removeFromTravelMap(int entityID)
{
	bool removed = false;

	for (unsigned int i = 0; i < m_travelMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_travelMap[i].size(); j++)
		{
			if (m_travelMap[i][j]->getEntityID() == entityID)
			{
				m_travelMap[i][j] = NULL;

				removed = true;
				break;
			}
		}

		if (removed)
		{
			break;
		}
	}
}

//=============================================================================
// Function: void moveCurrentX(int)
// Description:
// Moves the current x location on the map until it finds the next
// group. If there isn't one, it stays the same.
// Parameters:
// int amount - The amount to move the x by.
//=============================================================================
void UIMenu::moveCurrentX(int amount)
{
	int workingX = m_currentX + amount;
	int workingY = m_currentY;

	if (workingX < 0)
	{
		if (0 < workingY)
		{
			workingX = (int)m_travelMap.size() - 1;
			workingY -= 1;
		}
		else
		{
			workingX = 0;
		}
	}
	else if ((int)m_travelMap.size() <= workingX)
	{
		if (workingY < (int)m_travelMap[0].size() - 1)
		{
			workingX = 0;
			workingY += 1;
		}
		else
		{
			workingX = (int)m_travelMap.size() - 1;
		}
	}

	UIGroup *currentGroup = m_travelMap[workingX][workingY];

	while (!currentGroup)
	{
		workingX += amount;

		if (workingX < 0)
		{
			if (0 < workingY)
			{
				workingX = (int)m_travelMap.size() - 1;
				workingY -= 1;
			}
			else
			{
				workingX = 0;
				break;
			}
		}
		else if ((int)m_travelMap.size() <= workingX)
		{
			if (workingY < (int)m_travelMap[0].size() - 1)
			{
				workingX = 0;
				workingY += 1;
			}
			else
			{
				workingX = (int)m_travelMap.size() - 1;
				break;
			}
		}

		currentGroup = m_travelMap[workingX][workingY];
	}

	if (currentGroup)
	{
		m_currentX = workingX;
		m_currentY = workingY;
	}
}

//=============================================================================
// Function: void moveCurrentY(int)
// Description:
// Moves the current y position on the travel map until it finds the
// next group in the travel map. If a group can't be found,
// the current group stays unchanged.
// Parameters:
// int amount - The amount to move the current Y by.
//=============================================================================
void UIMenu::moveCurrentY(int amount)
{
	int workingX = m_currentX;
	int workingY = m_currentY + amount;

	if (workingY < 0)
	{
		workingY = 0;
	}
	else if ((int)m_travelMap[workingX].size() <= workingY)
	{
		workingY = (int)m_travelMap[workingX].size() - 1;
	}

	UIGroup *current = m_travelMap[workingX][workingY];

	while (!current)
	{
		workingY += amount;

		if (workingY < 0)
		{
			workingY = 0;
			break;
		}
		else if ((int)m_travelMap[workingX].size() <= workingY)
		{
			workingY = (int)m_travelMap[workingX].size() - 1;
			break;
		}

		current = m_travelMap[workingX][workingY];
	}

	if (current)
	{
		m_currentX = workingX;
		m_currentY = workingY;
	}
}

//=============================================================================
// Function: void setCurrentGroup(int)
// Description:
// Finds the group with the specified ID in the travel map and makes
// it the current group.
// Parameters:
// int entityID - The group ID.
//=============================================================================
void UIMenu::setCurrentGroup(int entityID)
{
	for (unsigned int i = 0; i < m_travelMap.size(); i++)
	{
		for (unsigned int j = 0; j < m_travelMap[i].size(); j++)
		{
			if (m_travelMap[i][j]->getEntityID() == entityID)
			{
				m_currentX = i;
				m_currentY = j;

				return;
			}
		}
	}
}

//=============================================================================
// Function: void processInputMsg(InputMessage*)
// Description:
// Handles input messages and updates the menu based off of them.
// Parameters:
// InputMessage *inputMsg - The input message to process.
//=============================================================================
void UIMenu::processInputMsg(InputMessage *inputMsg)
{
	if (inputMsg && m_active)
	{
		Uint32 passedTicks = SDL_GetTicks() - m_prevUpdate;

		UIGroup *current = getCurrentGroup();

		switch (inputMsg->m_inputType)
		{
		case InputMessage::INPUT_BUTTON:
		{
			InputButtonMessage *button =
				static_cast<InputButtonMessage*>(inputMsg);
			
			if (button->m_pressed && 
				m_UPDATE_COOLDOWN <= passedTicks)
			{
				switch (button->m_buttonCode)
				{
				case InputDevice::ANALOG_UP:
				{
					if (current && !current->getActive())
					{
						moveCurrentY(-1);
						m_prevUpdate = SDL_GetTicks();
						break;
					}
				}
				case InputDevice::ANALOG_RIGHT:
				{
					if (current && !current->getActive())
					{
						moveCurrentX(1);
						m_prevUpdate = SDL_GetTicks();
						break;
					}
				}
				case InputDevice::ANALOG_DOWN:
				{
					if (current && !current->getActive())
					{
						moveCurrentY(1);
						m_prevUpdate = SDL_GetTicks();
					}

					break;
				}
				case InputDevice::ANALOG_LEFT:
				{
					if (current && !current->getActive())
					{
						moveCurrentX(-1);
						m_prevUpdate = SDL_GetTicks();
					}

					break;
				}
				case InputDevice::SELECT:
				{
					if (button->m_deviceType ==
						InputDevice::KEYBOARD_MOUSE)
					{
						InputDevice *inputDevice = 
							InputSystem::instance()->getDevice(button->m_deviceID);
						
						if (inputDevice)
						{
							KeyboardMouse *kbm =
								static_cast<KeyboardMouse*>(inputDevice);

							CollisionSystem *collisionSys =
								PhysicsSystem::instance()->collisionSystem();

							UIGroup *insideGroup = pointInsideGroup(kbm->getPosition());

							if (insideGroup)
							{
								if (current)
								{
									current->setActive(false);
								}

								setCurrentGroup(insideGroup->getEntityID());

								insideGroup->setActive(true);
							}
						}
					}
					else
					{
						if (current)
						{
							current->setActive(true);
						}
					}
					break;
				}
				case InputDevice::BACK:
				{
					UIGroup *current = getCurrentGroup();

					if (current)
					{
						if (current->getActive())
						{
							current->setActive(false);
						}
					}

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

			if (m_UPDATE_COOLDOWN <= passedTicks)
			{
				if (current && !current->getActive())
				{
					if (axis->m_axis == SDL_CONTROLLER_AXIS_LEFTX)
					{
						if (axis->m_axisMovement < 0)
						{
							moveCurrentX(-1);
							m_prevUpdate = SDL_GetTicks();
						}
						else if (0 < axis->m_axisMovement)
						{
							moveCurrentX(1);
							m_prevUpdate = SDL_GetTicks();
						}
					}
					else if (axis->m_axis == SDL_CONTROLLER_AXIS_LEFTY)
					{
						if (axis->m_axisMovement < 0)
						{
							moveCurrentY(-1);
							m_prevUpdate = SDL_GetTicks();
						}
						else if (0 < axis->m_axisMovement)
						{
							moveCurrentY(1);
							m_prevUpdate = SDL_GetTicks();
						}
					}
				}
			}
			break;
		}
		}

		if (current)
		{
			current->processMessage(inputMsg);
		}
	}
}

//=============================================================================
// Function: void processEntityDestroyMsg(EntityDestroyMessage*)
// Description:
// Handles Entity Destroy messages and removes the group related to them.
// If it's not a group to destroy, it passes the message on to all of its
// groups.
// Parameters:
// EntityDestroyMessage *destroyMsg - The destroy message to process.
//=============================================================================
void UIMenu::processEntityDestroyMsg(EntityDestroyMessage *destroyMsg)
{
	if (destroyMsg)
	{
		removeGroup(destroyMsg->m_entityID);

		auto groupIt = m_groups.begin();

		while (groupIt != m_groups.end())
		{
			groupIt->second->processMessage(destroyMsg);

			groupIt++;
		}
	}
}

//=============================================================================
// Function: void processCameraMoveMsg(CameraMoveMessage*)
// Description:
// Processes the camera move message and moves the menu and its items.
// Parameters:
// CameraMoveMessage *moveMsg - The message to process.
//=============================================================================
void UIMenu::processCameraMoveMsg(CameraMoveMessage *moveMsg)
{
	if (moveMsg)
	{
		Vector2D offset = m_rect->center() - moveMsg->m_oldPosition;

		setPosition(moveMsg->m_newPosition + offset);
	}
}

//=============================================================================
// Function: UIGroup* pointInsideGroup(Vector2D point)
// Description:
// Checks all of the groups to see if the point is inside them.
// Parameters:
// Vector2D point - The point to check.
// Output:
// UIGroup*
// On success - Returns the UIGroup the point is inside.
// On failure - Returns NULL.
//=============================================================================
UIGroup* UIMenu::pointInsideGroup(Vector2D point)
{
	UIGroup *groupInside = NULL;

	auto groupIt = m_groups.begin();

	CollisionSystem *collisionSys =
		PhysicsSystem::instance()->collisionSystem();

	while (groupIt != m_groups.end())
	{
		if (collisionSys->pointInsideRect(groupIt->second->getRect(), (int)point.getX(), (int)point.getY()))
		{
			groupInside = groupIt->second;
			break;
		}

		groupIt++;
	}

	return groupInside;
}