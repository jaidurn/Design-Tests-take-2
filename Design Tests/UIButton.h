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
#include <string>
#include <SDL.h>

typedef void(*functionPtr)();
typedef std::string string;

class Font;

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

	void setText(string text, Font *font, SDL_Color color);

	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setActive(bool active);
	virtual void setPosition(Vector2D position);
	virtual void setVisible(bool visible);

	virtual void update(float delta);
	virtual void processMessage(IMessage *message);

	// Tools for setting up functionality.
	void setOnPress(functionPtr onPress);
	void setOnRelease(functionPtr onRelease);
	void setOnHold(functionPtr onHold);

protected:
	Shape::Rectangle *m_rect;

	functionPtr m_onPress;
	functionPtr m_onRelease;
	functionPtr m_onHold;

	bool m_wasPressed;
	bool m_pressed;
};

