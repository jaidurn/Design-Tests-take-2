#pragma once
//==========================================================================================
// File Name: UIList.h
// Author: Brian Blackmon
// Date Created: 9/19/2019
// Purpose: 
// Extends the UIGroup class and adds new functionality for visual lists.
//==========================================================================================
#include "UIGroup.h"
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

private:
	UIComponent *m_selected;
	int m_itemsVisible;
};

