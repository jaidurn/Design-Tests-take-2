#pragma once
//==========================================================================================
// File Name: TextureCache.h 
// Author: Brian Blackmon
// Date Created: 4/19/2019
// Purpose: 
// A cache class for creating and accessing textures.
//==========================================================================================
#include <map>

class Renderer;
class Window;
class Texture;

typedef std::string Key;

class TextureCache
{
public:
	TextureCache(Window *window, Renderer *renderer);
	~TextureCache();

	Texture* getTexture(Key key);

private:
	Window *m_window;
	Renderer *m_renderer;
	std::map<Key, Texture*> m_cache;

	void cleanUp();
	Texture* createTexture(Key key);
	bool colorKeyTexture(Texture *texture);
};