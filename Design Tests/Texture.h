#pragma once
//==========================================================================================
// File Name: Texture.h
// Author: Brian Blackmon
// Date Created: 4/19/2019
// Purpose: 
// Wraps the SDL_Texture struct and adds functionality
//==========================================================================================
#include "Vector2D.h"

struct SDL_Texture;

typedef unsigned __int32 Uint32;

class Texture
{
public:
	Texture(SDL_Texture *texture, Vector2D center);
	~Texture();

	SDL_Texture* texture() { return m_texture; }
	Vector2D center() { return m_center; }

	void setPixelFormat(Uint32 format) { m_pixelFormat = format; }
	Uint32 pixelFormat() { return m_pixelFormat; }

	int width() { return m_width; }
	int height() { return m_height; }
	int pitch() { return m_pitch; }
	void* pixels() { return m_pixels; }

	bool lock();
	bool unlock();

private:
	// TODO: Maybe add some instance information into this class?
	SDL_Texture *m_texture;
	Vector2D m_center;

	Uint32 m_pixelFormat;

	int m_width;
	int m_height;
	int m_pitch;
	
	void *m_pixels;
};

