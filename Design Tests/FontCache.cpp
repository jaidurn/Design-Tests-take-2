#include "FontCache.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include <iostream>

FontCache::FontCache(Renderer *renderer)
	:m_renderer(renderer)
{
	if(!m_renderer)
	{
		std::cout << "Error creating font cache, no renderer!\n";

		this->~FontCache();
	}
}

FontCache::~FontCache()
{
	cleanUp();
}

//=============================================================================
// Function: Font *getFont(string)
// Description:
// Finds the font with the specified path. If one doesn't exist,
// it gets created.
// Parameters:
// string fontPath - The location of the font files.
// Output:
// Returns a pointer to a font on success.
// Returns NULL on failure.
//=============================================================================
Font* FontCache::getFont(string fontPath,
	int pointSize, 
	Font::FontFlags flag,
	bool italics)
{
	Font *font = NULL;

	if (m_renderer)
	{
		auto mit = m_fonts.find(fontPath);

		if (mit == m_fonts.end())
		{
			font = new Font(fontPath, pointSize, flag, italics);
		}
		else
		{
			font = mit->second;

			font->setPointSize(pointSize);
			font->setFlag(flag);
			font->setItalic(italics);
		}
	}

	return font;
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up the existing items in the maps, deleting them.
//=============================================================================
void FontCache::cleanUp()
{
	auto fit = m_fonts.begin();

	while(fit != m_fonts.end())
	{
		delete fit->second;

		fit->second = NULL;

		fit = m_fonts.erase(fit);

		if(fit != m_fonts.end())
		{
			fit++;
		}
	}

	auto mit = m_textTextures.begin();

	while(mit != m_textTextures.end())
	{
		delete mit->second;
		mit->second = NULL;

		mit = m_textTextures.erase(mit);

		if(mit != m_textTextures.end())
		{
			mit++;
		}
	}
}