#pragma once
//==========================================================================================
// File Name: UIYList.h
// Author: Brian Blackmon
// Date Created: 9/23/2019
// Purpose: 
// Extends the UIGroup base class and adds functionality for a vertical list.
//==========================================================================================
#include "UIGroup.h"
#include <SDL.h>

class InputMessage;
class EntityDestroyMessage;

class UIYList : public UIGroup
{
public:
	UIYList(int entityID,
		string name,
		Vector2D position,
		int width, 
		int height, 
		int itemsVisible);
	virtual ~UIYList();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setActive(bool active);
	virtual void setVisible(bool visible);

	virtual GROUP_TYPE getGroupType() { return GROUP_YLIST; }

	virtual void addItem(UIComponent *item);
	virtual void removeItem(int entityID);
	virtual void removeItem(string itemName);

	virtual void processMessage(IMessage *message);
	virtual void update();

	UIComponent* getCurrentItem();

protected:
	Uint32 m_prevUpdate;
	const Uint32 m_UPDATE_COOLDOWN = 300;

	int m_itemsVisible;
	int m_itemOffset;

	virtual void cleanUp();
	virtual void updatePositions();
	virtual void adjustCurrentItem(int amount);

	virtual void processInputMsg(InputMessage *inputMsg); 
	virtual void processEntityDestroyMsg(EntityDestroyMessage *destroyMsg);
};

