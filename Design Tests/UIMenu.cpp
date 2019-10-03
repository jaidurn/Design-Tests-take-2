#include "UIMenu.h"
#include "UIGraphic.h"
#include "UIButton.h"
#include "UIText.h"

#include "MessageSystem.h"
#include "InputMessage.h"
#include "CameraMoveMessage.h"

#include "RenderSystem.h"

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
		else
		{
			UIGroup *current = getCurrentGroup();
			if (current)
			{
				current->setActive(m_active);
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

			group->setPosition(group->getPosition + m_rect->center());

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
// Function: UIGroup* getGroup(int)
// Description:
// Finds and returns a the group with the specified ID.
// Parameters:
// int entityID - The ID of the group to find.
// Output:
// UIGroup*
// On success - Returns a pointer to the found group.
// On failure - Returns NULL.
//=============================================================================
UIGroup* UIMenu::getGroup(int entityID)
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
// Function: UIGroup* getGroup(string)
// Description:
// Finds and returns the group with the specified name.
// Parameters:
// string name - The name of the group to find.
// Output:
// UIGroup*
// On success - Returns a pointer to the found group.
// On failure - Returns NULL.
//=============================================================================
UIGroup* UIMenu::getGroup(string name)
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
// Function: UIGroup* getCurrentGroup()
// Description:
// Gets the currently selected group.
// Output:
// UIGroup*
// On success - Returns a pointer to the current group.
// On failure - Returns NULL.
//=============================================================================
UIGroup* UIMenu::getCurrentGroup()
{
	UIGroup *group = NULL;

	group = m_travelMap[m_currentX][m_currentY];

	return group;
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

		if (m_itemsWide <= mapX)
		{
			int difference = mapX - m_itemsWide;

			m_travelMap.resize(mapX);

			for (int i = m_itemsWide - 1; i < mapX; i++)
			{
				for (int j = 0; j < m_itemsHigh; j++)
				{
					m_travelMap[i].push_back(NULL);
				}
			}
		}

		if (m_itemsHigh <= mapY)
		{
			int difference = mapY - m_itemsHigh;

			for (unsigned int i = 0; i < m_travelMap.size(); i++)
			{
				for (int j = 0; j <= difference; j++)
				{
					m_travelMap[i].push_back(NULL);
				}
			}
		}

		if (m_travelMap[mapX][mapY])
		{
		}
	}
}