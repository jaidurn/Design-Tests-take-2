#pragma once
//==========================================================================================
// File Name: FontCache.h
// Author: Brian Blackmon
// Date Created: 8/19/2019
// Purpose: 
// Loads and stores fonts that have been created.
//==========================================================================================
#include <map>
#include <string>
#include "Font.h"
#include "Texture.h"

class Renderer;

typedef std::string string;

class FontCache
{
public:
	FontCache(Renderer *renderer);
	~FontCache();

	Font* getFont(string fontPath, int pointSize, Font::FontFlags flag, bool italics);

private:
	std::map<string, Font*> m_fonts;

	Renderer *m_renderer;

	void cleanUp();
};

