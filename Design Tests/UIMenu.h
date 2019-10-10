#pragma once
//==========================================================================================
// File Name: UIMenu.h
// Author: Brian Blackmon
// Date Created: 9/19/2019
// Purpose: 
// The base class for menus. Adds basic menu storage and functionality.
//==========================================================================================
#include "UIGroup.h"
#include "Rectangle.h"
#include <SDL.h>
#include <map>
#include <vector>

class UIGraphic;
class CameraMoveMessage;
class InputMessage;
class InputButtonMessage;
class InputAxisMessage;
class InputMoveMessage;
class EntityDestroyMessage;

class UIMenu
{
public:
	UIMenu(int entityID,
		string name,
		Vector2D position,
		int width, 
		int height,
		int itemsWide,
		int itemsHigh);
	virtual ~UIMenu();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setVisible(bool visible);
	virtual void setActive(bool active);
	virtual void setDocked(bool docked);

	virtual void setBackground(UIGraphic *background);

	virtual void addGroup(UIGroup *group);
	virtual void removeGroup(int entityID);
	virtual void removeGroup(string name);

	UIGroup* getGroup(int entityID) const;
	UIGroup* getGroup(string name) const;

	UIGroup* getCurrentGroup() const;

	virtual void update();
	virtual void processMessage(IMessage *message);

protected:
	Rectangle *m_rect;
	UIGraphic *m_background;

	int m_itemsWide;
	int m_itemsHigh;

	int m_currentX;
	int m_currentY;

	Uint32 m_prevUpdate;
	const Uint32 m_UPDATE_COOLDOWN = 300;

	bool m_docked;
	bool m_visible;
	bool m_active;

	std::map<int, UIGroup*> m_groups;
	std::vector<std::vector<UIGroup*>> m_travelMap;

	virtual void cleanUp();
	virtual void insertGroup(UIGroup *group, int mapX, int mapY);
	virtual void removeFromTravelMap(int entityID);
	virtual void moveCurrentX(int amount);
	virtual void moveCurrentY(int amount);
	virtual void setCurrentGroup(int entityID);

	virtual void processInputMsg(InputMessage *inputMsg);
	virtual void processInputAxisMsg(InputAxisMessage *axisMsg);
	virtual void processInputButtonMsg(InputButtonMessage *buttonMsg);
	virtual void processInputMoveMsg(InputMoveMessage *moveMsg);
	virtual void processEntityDestroyMsg(EntityDestroyMessage *destroyMsg);
	virtual void processCameraMoveMsg(CameraMoveMessage *moveMsg);

	bool canUpdate();

	virtual UIGroup* pointInsideGroup(Vector2D point);
};

