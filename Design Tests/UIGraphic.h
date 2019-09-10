#pragma once
//==========================================================================================
// File Name: UIGraphic.h
// Author: Brian Blackmon
// Date Created: 8/22/2019
// Purpose: 
// Holds information about graphical UI.
//==========================================================================================
#include "UIComponent.h"
#include <SDL.h>
#include <string>
#include "Rectangle.h"

typedef Shape::Rectangle Rectangle;
typedef std::string string;

class UIGraphic : public UIComponent
{
public:
	UIGraphic(int entityID, Vector2D position, string graphicPath);
	virtual ~UIGraphic();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setVisible(bool visible);

	void setClip(SDL_Rect clip);
	void setRect(int width, int height);

	Rectangle* getRect() { return m_rect; }

	virtual void update(float delta);
	virtual void processMessage(IMessage *message);

private:
	Rectangle *m_rect;
};

