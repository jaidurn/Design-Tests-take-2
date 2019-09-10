#pragma once
//==========================================================================================
// File Name: UIComponent.h
// Author: Brian Blackmon
// Date Created: 8/15/2019
// Purpose: 
// The base class for UI components.
//==========================================================================================
#include "Component.h"
#include "Vector2D.h"

class IMessage;

class UIComponent : public Component
{
public:
	enum UI_TYPE
	{
		UI_BUTTON,
		UI_TEXT,
		UI_GRAPHIC,
	};

	UIComponent(UI_TYPE type, int entityID, Vector2D position);
	virtual ~UIComponent();

	UI_TYPE getType() { return m_type; }

	int getEntityID() { return m_entityID; }

	// Position
	Vector2D getPosition() { return m_position; }
	virtual void setPosition(Vector2D position) { m_position = position; }

	virtual void setWidth(int width) = 0;
	virtual void setHeight(int height) = 0;

	bool getActive() { return m_active; }
	virtual void setActive(bool active) { m_active = active; }

	bool getDocked() { return m_docked; }
	void setDocked(bool docked) { m_docked = docked; }

	bool getVisible() { return m_visible; }
	virtual void setVisible(bool visible) = 0;

	// Logic
	virtual void update(float delta) = 0;
	virtual void processMessage(IMessage *message) = 0;

protected:
	int m_entityID;
	bool m_active;
	bool m_docked;
	bool m_visible;

	Vector2D m_position;

private:
	UI_TYPE m_type;
};

