#pragma once
//==========================================================================================
// File Name: UIGroup.h
// Author: Brian Blackmon
// Date Created: 9/19/2019
// Purpose: 
// The base class for groups of UIComponents.
//==========================================================================================
#include <vector>
#include <string>
#include "Vector2D.h"
#include "Rectangle.h"

class UIComponent;
class UIGraphic;
class IMessage;

typedef Shape::Rectangle Rectangle;
typedef std::string string;

class UIGroup
{
public:
	UIGroup(int entityID, Vector2D position, int width, int height);
	virtual ~UIGroup();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setActive(bool active);
	virtual void setVisible(bool visible);

	virtual void setBackground(UIGraphic *background);

	Vector2D getPosition();
	int getWidth();
	int getHeight();
	bool getActive();
	bool getVisible();

	virtual void addItem(UIComponent *item);
	virtual void removeItem(int entityID);
	virtual void removeItem(string itemName);
	virtual UIComponent* getItem(int entityID);
	virtual UIComponent* getItem(string itemName);

	virtual void processMessage(IMessage *message);
	virtual void update();

protected:
	Rectangle *m_rect;
	UIGraphic *m_background;

	int m_entityID;

	bool m_active;
	bool m_visible;

	std::vector<UIComponent*> m_items;

	virtual void cleanUp();
};
