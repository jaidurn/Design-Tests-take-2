#pragma once
//==========================================================================================
// File Name: UIMenu.h
// Author: Brian Blackmon
// Date Created: 8/22/2019
// Purpose: 
// Holds information and functionality for menus. 
//==========================================================================================
#include "UIComponent.h"
#include "UIGraphic.h"
#include "UIButton.h"
#include "UIText.h"
#include <map>
#include "Rectangle.h"
#include <string>

typedef Shape::Rectangle Rectangle;
typedef std::string string;

class UIMenu
{
public:
	UIMenu(Vector2D position, int width, int height, string texturePath);
	~UIMenu();

	virtual void setPosition(Vector2D position);
	virtual Vector2D getPosition();

	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setActive(bool active);
	virtual void setVisible(bool visible);

	virtual int getWidth();
	virtual int getHeight();

	bool getVisible() { return m_visible; }
	bool getActive() { return m_active; }
	
	void toggleVisible() { setVisible(!m_visible); }

	virtual void update(float delta);
	virtual void processMessage(IMessage *message);

	virtual void addItem(int itemID, UIComponent *item);
	virtual void removeItem(int itemID);
	virtual UIComponent* getItem(int itemID);

protected:
	std::map<int, UIComponent*> m_elements;
	
	Rectangle *m_rect;

	int m_backgroundID;

	bool m_active;
	bool m_visible;

	void cleanUp();
};

