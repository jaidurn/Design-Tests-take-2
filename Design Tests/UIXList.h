#pragma once
//==========================================================================================
// File Name: UIList.h
// Author: Brian Blackmon
// Date Created: 9/19/2019
// Purpose: 
// Extends the UIGroup class and adds new functionality for visual lists.
//==========================================================================================
#include "UIGroup.h"
#include <SDL.h>

class EntityDestroyMessage;
class InputMessage;

class UIXList : public UIGroup
{
public:
	UIXList(int entityID, Vector2D position, int width, int height, int itemsVisible);
	virtual ~UIXList();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);

	virtual void addItem(UIComponent *item);

	virtual void processMessage(IMessage *message);
	virtual void update();

	UIComponent* getCurrentItem();

protected:
	int m_currentItem;
	int m_itemsVisible;
	int m_itemOffset;
	
	Uint32 m_prevUpdate;
	const Uint32 m_UPDATE_COOLDOWN = 500;

	virtual void cleanUp();
	virtual void updatePositions();
	virtual void adjustCurrentItem(int amount);

	virtual void processInput(InputMessage *inputMsg);
	virtual void processEntityDestroy(EntityDestroyMessage *destroyMsg);
};

