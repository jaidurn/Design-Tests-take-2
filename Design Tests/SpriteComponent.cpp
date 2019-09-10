#include "SpriteComponent.h"
#include <cassert>
#include <iostream>

SpriteComponent::SpriteComponent(Texture *texture)
	:Component(SPRITE),
	m_texture(texture),
	m_anchor(-1, -1),
	m_position(0, 0),
	m_rotation(0.0f),
	m_visible(true),
	m_layer(0),
	m_scaleable(true)
{
	m_colorMod.r = 0;
	m_colorMod.g = 0;
	m_colorMod.b = 0;
	m_colorMod.a = 0;

	m_clip.x = 0; m_clip.y = 0;
	m_clip.w = 0; m_clip.h = 0;

	m_renderSize.x = 0; m_renderSize.y = 0;
	m_renderSize.w = 0; m_renderSize.h = 0;

	m_blendMode = SDL_BLENDMODE_BLEND;

	// Default the anchor to the center
	if (texture) 
	{ 
		m_anchor = m_texture->center(); 

		m_clip.w = m_texture->width();
		m_clip.h = m_texture->height();

		m_renderSize.w = m_texture->width();
		m_renderSize.h = m_texture->height();
	
		if(SDL_GetTextureColorMod(texture->texture(), &m_colorMod.r, &m_colorMod.g, &m_colorMod.b) != 0)
		{
			std::cout << SDL_GetError() << std::endl;
			
			m_colorMod.r = 0;
			m_colorMod.g = 0;
			m_colorMod.b = 0;
		}
		
		if(SDL_GetTextureAlphaMod(texture->texture(), &m_colorMod.a) != 0)
		{
			std::cout << SDL_GetError() << std::endl;

			m_colorMod.a = 255;
		}

		if (SDL_GetTextureBlendMode(texture->texture(), &m_blendMode) != 0)
		{
			std::cout << "Error getting blend mode: " << SDL_GetError() << std::endl;

			m_blendMode = SDL_BLENDMODE_BLEND;
		}

		if(m_blendMode != SDL_BLENDMODE_BLEND)
		{
			std::cout << "WHAT\n";
		}
	}
}

SpriteComponent::~SpriteComponent()
{
	// TODO: Tell texture something is unsubscribing!
	m_texture = NULL;
}

//=============================================================================
// Function: void setTexture(Texture)
// Description:
// Sets the texture pointer to the texture provided. If the anchor hasn't been
// set already, it sets the anchor to the center of the texture.
// Parameters:
// Texture *texture - The texture to set the member texture to.
//=============================================================================
void SpriteComponent::setTexture(Texture *texture)
{
	if (texture)
	{
		m_texture = texture;

		// Make sure the anchor gets initialized.
		if(m_anchor.getX() < 0 || m_anchor.getY() < 0)
		{
			m_anchor = m_texture->center();
		}
	}
}

//=============================================================================
// Function: void setClip(int, int, int, int)
// Description:
// Sets the texture clip of the sprite.
// Parameters:
// int x - The x position of the clip.
// int y - The y position of the clip.
// int w - The width of the clip.
// int h - The height of the clip.
//=============================================================================
void SpriteComponent::setClip(int x, int y, int w, int h)
{
	if(m_renderSize.w == m_clip.w &&
		m_renderSize.h == m_clip.h)
	{
		if (0 < w) { m_renderSize.w = w; }
		if (0 < h) { m_renderSize.h = h; }
	}

	if (0 <= x) { m_clip.x = x; }
	if (0 <= y) { m_clip.y = y; }
	if (0 < w) { m_clip.w = w; }
	if (0 < h) { m_clip.h = h; }
}

//=============================================================================
// Function: void setRenderSize(int, int)
// Description:
// Sets the size to render the sprite at.
// Parameters:
// int width - The width of the sprite.
// int height - The height of the sprite.
//=============================================================================
void SpriteComponent::setRenderSize(int width, int height)
{
	if(0 < width)
	{
		m_renderSize.w = width;
	}
	
	if(0 < height)
	{
		m_renderSize.h = height;
	}
}