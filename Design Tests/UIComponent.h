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
		UI_LIST,
		UI_GRAPHIC,
	};

	UIComponent(UI_TYPE type, int entityID, Vector2D position);
	virtual ~UIComponent();

	UI_TYPE getType() { return m_type; }

	// Position
	Vector2D getPosition() { return m_position; }
	virtual void setPosition(Vector2D position) { m_position = position; }

	bool getActive() { return m_active; }
	void setActive(bool active) { m_active = active; }

	bool getDocked() { return m_docked; }
	void setDocked(bool docked) { m_docked = docked; }

	// Logic
	virtual void update(float delta) = 0;
	virtual void processMessage(IMessage *message) = 0;

protected:
	int m_entityID;
	bool m_active;
	bool m_docked;

	Vector2D m_position;

private:
	UI_TYPE m_type;
};

