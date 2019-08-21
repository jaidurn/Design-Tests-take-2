#pragma once
//==========================================================================================
// File Name: ResoureManager.h
// Author: Brian Blackmon
// Date Created: 4/20/2019
// Purpose: 
// Holds all the resource caches.
//==========================================================================================
#include "Window.h"
#include "Renderer.h"
#include "TextureCache.h"
#include "FontCache.h"

class ResourceManager
{
public:
	
	static ResourceManager* instance()
	{
		static ResourceManager *instance = new ResourceManager();

		return instance;
	}

	~ResourceManager();

	Texture* getTexture(Key key);
	Texture* getTextTexture(string text, Font *font, SDL_Color color, Uint32 *wrapLength);

	Font* getFont(string fontPath, int pointSize, Font::FontFlags flag, bool italics);

	Renderer* renderer() { return m_renderer; }
	Window* window() { return m_window; }

	bool windowInitialized();
	bool rendererInitialized();

	void initWindow(std::string title, int width, int height, Uint32 flags);
	void initRenderer(bool vSyncEnabled);

private:
	ResourceManager()
		:m_window(NULL), m_renderer(NULL), m_textureCache(NULL)
	{}

	Window *m_window;
	Renderer *m_renderer;
	TextureCache *m_textureCache;
	FontCache *m_fontCache;

};

