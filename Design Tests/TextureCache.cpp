#include "TextureCache.h"
#include "Texture.h"
#include "Window.h"
#include "Renderer.h"
#include <SDL_image.h>
#include <cassert>
#include <iostream>
#include <string>
#include "SpriteParser.h"

TextureCache::TextureCache(Window *window, Renderer *renderer)
	:m_window(window), m_renderer(renderer)
{
	assert(m_renderer != NULL && m_window != NULL);
}

TextureCache::~TextureCache()
{
	cleanUp();
}

//=============================================================================
// Function: Texture* getTexture(Key)
// Description:
// Looks for a texture with a corresponding key. If one isn't found,
// it creates one.
//
// Parameters:
// Key key - The texture's file location.
//
// Output:
// Texture* - Outputs the texture from the cache. Returns NULL on failure.
//=============================================================================
Texture* TextureCache::getTexture(Key key)
{
	Texture *texture = NULL;

	auto mit = m_cache.find(key);

	// If the texture already exists, just return the texture.
	if (mit != m_cache.end())
	{
		texture = mit->second;
	}
	// Otherwise, create the texture
	else
	{
		texture = createTexture(key);

		if(texture)
		{
			m_cache.insert(std::make_pair(key, texture));
		}
	}

	if(!texture)
	{
		std::cout << "Failed to get texture with key: " << key << std::endl;
	}

	return texture;
}

//=============================================================================
// Function: void TextureCache cleanUp()
// Description:
// Cleans up the texture cache and deletes all of the saved textures.
//=============================================================================
void TextureCache::cleanUp()
{
	auto mit = m_cache.begin();

	while(mit != m_cache.end())
	{
		delete mit->second;
		mit = m_cache.erase(mit);
		
		if(mit != m_cache.end())
		{
			mit++;
		}
	}
}

//=============================================================================
// Function: Texture* createTexture(Key)
// Description:
// Creates the texture using the key provided and adds it to the cache.
// Parameters:
// Key key - The folder location of the texture.
// Output:
// Texture* - A pointer to the created texture. Returns NULL on failure.
//=============================================================================
Texture* TextureCache::createTexture(Key key)
{
	Texture *texture = NULL;

	SDL_Surface *temp = IMG_Load(key.c_str());

	if (temp)
	{
		Uint32 pixelFormat = m_renderer->getAlphaPixelFormat();

		if (pixelFormat != -1)
		{
			SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(temp, pixelFormat, NULL);

			int width = formattedSurface->w;
			int height = formattedSurface->h;

			if (formattedSurface)
			{
				SDL_Texture *blankTexture = SDL_CreateTexture(m_renderer->renderer(), pixelFormat, SDL_TEXTUREACCESS_STREAMING, width, height);

				if (blankTexture)
				{
					void *pixels = NULL;
					int pitch = 0;

					SDL_LockTexture(blankTexture, NULL, &pixels, &pitch);

					// Copy the loaded surface pixels
					memcpy(pixels, formattedSurface->pixels, formattedSurface->pitch * height);

					SDL_UnlockTexture(blankTexture);
					pixels = NULL;

					float centerX = (float)width / 2.0f;
					float centerY = (float)height / 2.0f;

					texture = new Texture(blankTexture, Vector2D(centerX, centerY));
					texture->setPixelFormat(pixelFormat);

					if(!colorKeyTexture(texture))
					{
						std::cout << "Color keying failed!\n";
					}
				}
				else
				{
					std::cout << "Unable to create blank texture! SDL Error: "
						<< SDL_GetError()
						<< std::endl;
				}

				SDL_FreeSurface(formattedSurface);
			}
			else
			{
				std::cout << "Unable to convert loaded surface to display format! SDL Error: "
					<< IMG_GetError()
					<< std::endl;
			}

		}
		else
		{
			SDL_PixelFormat *format = SDL_AllocFormat(m_window->pixelFormat());
			Uint32 *pixel = (Uint32*)temp->pixels;
			Uint32 colorKey = SDL_MapRGB(format, 0xC8, 0xF4, 0xC0);

			if(SDL_SetColorKey(temp, SDL_TRUE, colorKey) != 0)
			{
				std::cout << "Failed to set color key! SDL Error: "
					<< SDL_GetError();
			}

			SDL_Texture *s_texture = SDL_CreateTextureFromSurface(m_renderer->renderer(), temp);

			if (s_texture)
			{
				texture = new Texture(s_texture, Vector2D(temp->w / 2, temp->h / 2));
			}
			else
			{
				std::cout << "Failed to create the texture!\n";
			}

			SDL_FreeFormat(format);
		}

		// Don't forget to free our surface.
		SDL_FreeSurface(temp);
	}
	else
	{
		std::cout << "Failed to create surface with key: " << key << std::endl;
	}

	return texture;
}

bool TextureCache::colorKeyTexture(Texture *texture)
{
	bool success = false;

	if (texture->lock())
	{
		if (SDL_ISPIXELFORMAT_ALPHA(texture->pixelFormat()) != 0)
		{
			SDL_PixelFormat *mappingFormat = SDL_AllocFormat(texture->pixelFormat());

			Uint32 *pixelData = (Uint32*)texture->pixels();
			int pixelCount = (texture->pitch() / 4) * texture->height();

			Uint32 colorKey = SDL_MapRGB(mappingFormat, 0xC8, 0xF4, 0xC0);
			Uint32 transparent = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0x00, 0x00);

			for (int i = 0; i < pixelCount; i++)
			{
				if (pixelData[i] == colorKey)
				{
					pixelData[i] = transparent;
				}
			}

			SDL_FreeFormat(mappingFormat);

			success = true;
		}
	}

	return success;
}