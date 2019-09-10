#pragma once
//==========================================================================================
// File Name: UIText.h
// Author: Brian Blackmon
// Date Created: 8/22/2019
// Purpose: 
// Holds information about a text component.
//==========================================================================================
#include "UIComponent.h"
#include <SDL.h>
#include <string>

class Font;

typedef std::string string;

class UIText : public UIComponent
{
public:
	UIText(int entityID, Vector2D position, string text, SDL_Color color, Font *font);
	virtual ~UIText();

	void setFont(Font *font);

	virtual void setWidth(int width);
	virtual void setHeight(int height);

	virtual void setPosition(Vector2D position);
	virtual void setVisible(bool visible);

	void setText(string text);
	void setColor(SDL_Color color);
	void setWrapWidth(Uint32 wrapWidth);

	virtual void update(float delta);
	virtual void processMessage(IMessage *message);
};


