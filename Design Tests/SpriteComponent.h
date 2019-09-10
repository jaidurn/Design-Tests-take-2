#pragma once
//==========================================================================================
// File Name: SpriteComponent.h
// Author: Brian Blackmon
// Date Created: 4/19/2019
// Purpose: 
// Stores information pertaining to the Sprite.
//==========================================================================================
#include "Component.h"
#include "Vector2D.h"
#include "Texture.h"
#include <SDL.h>

class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(Texture *texture);
	~SpriteComponent();

	void setTexture(Texture *texture);
	void setAnchor(Vector2D anchor) { m_anchor = anchor; }
	void setPosition(Vector2D position) { m_position = position; }
	void setRotation(float rotation) { m_rotation = rotation; }
	void setColorMod(SDL_Color colorMod) { m_colorMod = colorMod; }
	void setClip(int x, int y, int w, int h);
	void setVisible(bool visible) { m_visible = visible; }
	void setLayer(int layer) { m_layer = layer; }
	void setScaleable(bool scaleable) { m_scaleable = scaleable; }
	void setRenderSize(int width, int height);

	Texture* texture() { return m_texture; }
	SDL_Color colorMod() { return m_colorMod; }
	SDL_BlendMode blendMode() { return m_blendMode; }
	SDL_Rect& clip() { return m_clip; }
	SDL_Rect& renderSize() { return m_renderSize; }

	Vector2D anchor() { return m_anchor; }
	Vector2D position() { return m_position; }
	float rotation() { return m_rotation; }
	int width() { return m_texture->width(); }
	int height() { return m_texture->height(); }
	int layer() { return m_layer; }
	bool visible() { return m_visible; }
	bool scaleable() { return m_scaleable; }

private:
	Texture *m_texture;
	
	SDL_Color m_colorMod;
	SDL_BlendMode m_blendMode;
	SDL_Rect m_clip;
	SDL_Rect m_renderSize;

	Vector2D m_anchor;
	Vector2D m_position;
	
	float m_rotation;
	bool m_visible;
	bool m_scaleable;

	int m_layer;
};