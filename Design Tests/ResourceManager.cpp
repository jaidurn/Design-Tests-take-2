#include "ResourceManager.h"
#include <cassert>
#include <iostream>

ResourceManager::~ResourceManager()
{
	delete m_fontCache;
	delete m_textureCache;
	delete m_renderer;
	delete m_window;

	m_fontCache = NULL;
	m_textureCache = NULL;
	m_renderer = NULL;
	m_window = NULL;
}

Texture* ResourceManager::getTexture(Key key)
{
	if(!m_textureCache)
	{
		return NULL;
	}

	return m_textureCache->getTexture(key);
}

Texture* ResourceManager::getTextTexture(string text,
	Font *font,
	SDL_Color color,
	Uint32 *wrapLength)
{
	Texture *texture = NULL;

	if(m_fontCache)
	{
		texture = m_fontCache->getTextTexture(text, font, color, wrapLength);
	}

	return texture;
}

Font* ResourceManager::getFont(string fontPath,
	int pointSize,
	Font::FontFlags flag,
	bool italics)
{
	Font *font = NULL;

	if(m_fontCache)
	{
		font = m_fontCache->getFont(fontPath, pointSize, flag, italics);
	}

	return font;
}

bool ResourceManager::windowInitialized()
{
	if(m_window)
	{
		return true;
	}

	return false;
}

bool ResourceManager::rendererInitialized()
{
	if(m_renderer)
	{
		return true;
	}

	return false;
}

void ResourceManager::initWindow(std::string title, int width, int height, Uint32 flags)
{
	if(!m_window)
	{
		m_window = new Window(title, width, height, flags);
	}
}

void ResourceManager::initRenderer(bool vSyncEnabled)
{
	if(windowInitialized() && !rendererInitialized())
	{
		m_renderer = new Renderer(m_window->window(), vSyncEnabled);

		if(!m_textureCache)
		{
			m_textureCache = new TextureCache(m_window, m_renderer);
		}

		if(!m_fontCache)
		{
			m_fontCache = new FontCache(m_renderer);
		}
	}
}

