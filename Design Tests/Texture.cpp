#include "Texture.h"
#include <iostream>
#include <SDL.h>
#include <cassert>

Texture::Texture(SDL_Texture *texture, Vector2D center)
	:m_texture(texture), m_center(center), m_width(0), m_height(0), m_pitch(0), m_pixels(NULL), m_pixelFormat(-1)
{
	assert(texture != NULL);

	SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
	SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
}

Texture::~Texture()
{
	if (m_texture) { SDL_DestroyTexture(m_texture); }
}

bool Texture::lock()
{
	bool locked = false;

	if(!m_pixels)
	{
		if(SDL_LockTexture(m_texture, NULL, &m_pixels, &m_pitch) == 0)
		{
			locked = true;
		}
		else
		{
			std::cout << "Unable to lock texture!"
				<< SDL_GetError()
				<< std::endl;
		}
	}

	return locked; 
}

bool Texture::unlock()
{
	bool unlocked = false;

	if(m_pixels)
	{
		SDL_UnlockTexture(m_texture);
		m_pixels = NULL;
		m_pitch = 0;
	}

	return unlocked;
}
