#pragma once
//==========================================================================================
// File Name: UIText.h
// Author: Brian Blackmon
// Date Created: 9/12/2019
// Purpose: 
// Extends the UIComponent and adds text capabilities.
//==========================================================================================
#include "UIComponent.h"
#include <SDL.h>

class Font;

class UIText : public UIComponent
{
public:
	UIText(int entityID, Vector2D position, string name);
	virtual ~UIText();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setVisible(bool visible);

	void setFont(Font *font);
	void setText(string text);
	void setColor(SDL_Color color);
};

