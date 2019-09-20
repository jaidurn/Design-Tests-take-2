#pragma once
//==========================================================================================
// File Name: UIComponent.h
// Author: Brian Blackmon
// Date Created: 9/11/2019
// Purpose: 
// Holds the base information for UI Components. Extends the Component class.
//==========================================================================================
#include "Component.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include <string>

typedef Shape::Rectangle Rectangle;
typedef std::string string;

class UIComponent :	public Component
{
public:
	enum UI_Type
	{
		UI_BUTTON,
		UI_GRAPHIC,
		UI_TEXT
	};

	UIComponent(int entityID, UI_Type type, Vector2D position, string name);
	virtual ~UIComponent();
	
	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);	
	virtual void setName(string name);
	virtual void setVisible(bool visible);
	virtual void setActive(bool active);

	virtual Vector2D getPosition();
	virtual int getWidth();
	virtual int getHeight();
	virtual string getName();
	virtual bool getVisible();

	int getEntityID() { return m_entityID; }
	UI_Type getType() { return m_type; }

protected:
	Rectangle *m_rect;

	int m_entityID;
	string m_name;
	bool m_visible;
	bool m_active;
	UI_Type m_type;
};

