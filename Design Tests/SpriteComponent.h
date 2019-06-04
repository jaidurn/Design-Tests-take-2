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

	Texture* texture() { return m_texture; }
	Vector2D anchor() { return m_anchor; }
	Vector2D position() { return m_position; }
	float rotation() { return m_rotation; }
	int width() { return m_texture->width(); }
	int height() { return m_texture->height(); }

private:
	Texture *m_texture;
	Vector2D m_anchor;
	Vector2D m_position;
	float m_rotation;
};