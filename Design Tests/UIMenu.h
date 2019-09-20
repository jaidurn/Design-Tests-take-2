#pragma once
//==========================================================================================
// File Name: UIMenu.h
// Author: Brian Blackmon
// Date Created: 9/19/2019
// Purpose: 
// The base class for menus. Adds basic menu storage and functionality.
//==========================================================================================
#include "UIComponent.h"
#include "Rectangle.h"
#include <map>
#include "Node.h"

class UIGraphic;

class UIMenu
{
public:
	UIMenu(Vector2D position, int width, int height);
	virtual ~UIMenu();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);

	virtual void setBackground(UIGraphic *background);

	virtual void addItem(UIComponent *item);
	virtual void removeItem(int entityID);

protected:
	Rectangle *m_rect;

	UIGraphic *m_background;

	Node<UIComponent*> *m_items;
};

