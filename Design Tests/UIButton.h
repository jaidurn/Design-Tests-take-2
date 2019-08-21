#pragma once
//==========================================================================================
// File Name: UIButton.h
// Author: Brian Blackmon
// Date Created: 8/15/2019
// Purpose: 
// Handles information about a button.
//==========================================================================================
#include "UIComponent.h"
#include "Rectangle.h"

typedef void(*updatePtr)();

class UIButton : public UIComponent
{
public:
	UIButton(int entityID, Vector2D postion, Shape::Rectangle *rect, std::string buttonTexture);
	virtual ~UIButton();

	bool getPressed();
	bool getReleased();
	bool getHeld();
	void setPressed(bool pressed);

	Shape::Rectangle *getRect() { return m_rect; }
	void setRect(Shape::Rectangle *rect);

	virtual void setPosition(Vector2D position);

	virtual void update(float delta);
	virtual void processMessage(IMessage *message);

	// Tools for setting up functionality.
	void setUpdateFunction(updatePtr update);

protected:
	Shape::Rectangle *m_rect;

	updatePtr m_update;

	bool m_wasPressed;
	bool m_pressed;
};

