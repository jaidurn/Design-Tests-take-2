#pragma once
//==========================================================================================
// File Name: TextComponent.h
// Author: Brian Blackmon
// Date Created: 8/21/2019
// Purpose: 
// Allows for creation and manipulation of text items.
//==========================================================================================
#include "Component.h"
#include "Vector2D.h"
#include <string>
#include <SDL.h>

class Font;
class Texture;

typedef std::string string;

class TextComponent : public Component
{
public:
	TextComponent(Font *font, SDL_Color color, string text, Uint32 wrapWidth, Vector2D position);
	~TextComponent();

	void setText(string text);
	void setColor(SDL_Color color);
	void setColor(int r, int g, int b, int a);
	void setFont(Font *font);
	void setWrapWidth(Uint32 width);
	void setPosition(Vector2D position) { m_position = position; }
	void setVisible(bool visible) { m_visible = visible; }
	void setWidth(int width);
	void setHeight(int height);

	Font* getFont() { return m_font; }
	Texture* getTexture() { return m_texture; }
	SDL_Color getColor() { return m_color; }
	string getText() { return m_text; }
	Uint32 getWrapWidth() { return m_wrapWidth; }
	Vector2D getPosition() { return m_position; }
	bool getVisible() { return m_visible; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }

private:
	Font *m_font;
	Texture *m_texture;

	SDL_Color m_color;
	Vector2D m_position;

	string m_text;
	Uint32 m_wrapWidth;
	bool m_visible;

	int m_width;
	int m_height;

	void createTexture();
	void destroyTexture();
	void fixTextSize();
};

