#include "Font.h"
#include <iostream>


Font::Font(string fontPath, int pointSize, FontFlags fontFlag, bool italic)
	:m_font(false), 
	m_fontPath(fontPath), 
	m_pointSize(pointSize),
	m_flag(fontFlag),
	m_italic(italic)
{
	if(!createFont())
	{
		std::cout << "Failed to create font!\n";
	}
}

Font::~Font()
{
	closeFont();
}

//=============================================================================
// Function: bool isBold()
// Description:
// Checks if the font is bolded or not.
// Output:
// Returns true if bold
// Returns false if not bold
//=============================================================================
bool Font::isBold()
{
	bool bold = false;

	if (m_flag == FONT_BOLD) { bold = true; }

	return bold;
}

//=============================================================================
// Function: bool isItalic()
// Description:
// Checks if the font is italic or not.
// Output:
// Returns true if the font is italicized.
// Returns false if the font isn't italicized.
//=============================================================================
bool Font::isItalic()
{
	return m_italic;
}

//=============================================================================
// Function: bool isGlyphProvided(Uint16)
// Description:
// Checks to see if the font contains a glyph for the provided character.
// Parameters:
// Uint16 glyph - The glyph to find.
// Output:
// Returns true if the font has a glyph for the character.
// Returns false if the glyph doesn't exist.
//=============================================================================
bool Font::isGlyphProvided(Uint16 glyph)
{
	bool provided = false;

	if (m_font)
	{
		if (TTF_GlyphIsProvided(m_font, glyph))
		{
			provided = true;
		}
	}

	return provided;
}

//=============================================================================
// Function: int getOutline()
// Description:
// Gets the outline of the current font.
// Output:
// Returns the outline on success.
// Returns -1 on failure.
//=============================================================================
int Font::getOutline()
{
	int outline = -1;

	if (m_font)
	{
		outline = TTF_GetFontOutline(m_font);
	}

	return outline;
}

//=============================================================================
// Function: int getHeight()
// Description:
// Gets the maximum height of the font.
// Output:
// Returns the height on success.
// Returns -1 on failure.
//=============================================================================
int Font::getHeight()
{
	int height = -1;

	if(m_font)
	{
		height = TTF_FontHeight(m_font);
	}

	return height;
}

//=============================================================================
// Function: int getAscent()
// Description:
// Gets the ascent of the font.
// Output:
// Returns the ascent on success.
// Returns -1 on failure.
//=============================================================================
int Font::getAscent()
{
	int ascent = -1;

	if(m_font)
	{
		ascent = TTF_FontAscent(m_font);
	}

	return ascent;
}

//=============================================================================
// Function: int getDescent()
// Description:
// Gets the descent of the font.
// Output:
// Returns the descent on success.
// Returns -1 on failure.
//=============================================================================
int Font::getDescent()
{
	int descent = -1;

	if(m_font)
	{
		descent = TTF_FontDescent(m_font);
	}

	return descent;
}

//=============================================================================
// Function: int getLineSkip()
// Description:
// Gets the recommended pixel height of a rendered line of text for the font.
// Output:
// Returns the pixel height of a line on success.
// Returns -1 on failure.
//=============================================================================
int Font::getLineSkip()
{
	int lineSkip = -1;

	if(m_font)
	{
		lineSkip = TTF_FontLineSkip(m_font);
	}

	return lineSkip;
}

//=============================================================================
// Function: int getGlyphMetrics(Uint16, int*, int*, int*, int*, int*)
// Description:
// Gets the metrics for a glyph. 
// Parameters:
// Uint16 glyph - The character to get metrics for.
// int *minX - The minimum x position of the glyph.
// int *minY - The minimum y position of the glyph.
// int *maxX - The maximum x position of the glyph.
// int *maxY - The maximum y position of the glyph.
// int *advance - The advance of the glyph.
// Output:
// Fills all of the pointers with the correct data on success.
// Returns 0 on success.
// Returns -1 on errors.
//=============================================================================
int Font::getGlyphMetrics(Uint16 glyph,
	int *minX,
	int *minY,
	int *maxX,
	int *maxY,
	int *advance)
{
	int success = -1;

	if(m_font)
	{
		success = TTF_GlyphMetrics(m_font, glyph, minX, minY, maxX, maxY, advance);
	}
	
	return success;
}

//=============================================================================
// Function: int getTextSize(string, int*, int*)
// Description:
// Gets the render size of the text provided.
// Parameters:
// string text - The text to get the size of.
// int *width - The width of the text.
// int *height - The height of the text.
// Output:
// Returns 0 on success and fills the width and height.
// Returns -1 on failure.
//=============================================================================
int Font::getTextSize(string text, int *width, int *height)
{
	int success = -1;

	if(m_font)
	{
		success = TTF_SizeText(m_font, text.c_str(), width, height);
	}

	return success;
}

//=============================================================================
// Function: void setItalic(bool)
// Description:
// Sets the italic state and changes font if need be.
// Parameters:
// bool italic - The state of italics
//=============================================================================
void Font::setItalic(bool italic)
{
	// If we're changing italics
	if(italic != m_italic)
	{
		m_italic = italic;

		if(!createFont())
		{
			std::cout << "Failed to change the italics!\n";
		}
	}
}

//=============================================================================
// Function: void setFlag(FontFlags)
// Description:
// Sets the font flag.
// Parameters:
// FontFlags flag - The flag to change the font to.
//=============================================================================
void Font::setFlag(FontFlags flag)
{
	if(m_flag != flag)
	{
		m_flag = flag;

		if(!createFont())
		{
			std::cout << "Failed to chagne the font flag!\n";
		}
	}
}

//=============================================================================
// Function: void setPointSize(int)
// Description:
// Changes the font to a different point size.
// Parameters:
// int pointSize - The size to load the font at.
//=============================================================================
void Font::setPointSize(int pointSize)
{
	if(0 < pointSize && m_pointSize != pointSize)
	{
		int oldPointSize = m_pointSize;

		m_pointSize = pointSize;
		
		if(!createFont())
		{
			m_pointSize = oldPointSize;
		}
	}
}

//=============================================================================
// Function: bool changeFont()
// Description:
// Creates the font based on the current settings.
// Output:
// Returns true on success.
// Returns false on failure.
//=============================================================================
bool Font::createFont()
{
	TTF_Font *oldFont = NULL;

	if(m_font)
	{
		oldFont = m_font;
		closeFont();
	}

	bool success = true;

	string fontPath = m_fontPath;

	switch (m_flag)
	{
	case FONT_THIN:
	{
		fontPath += "Thin";
		break;
	}
	case FONT_LIGHT:
	{
		fontPath += "Light";
		break;
	}
	case FONT_REGULAR:
	{
		fontPath += "Regular";
		break;
	}
	case FONT_MEDIUM:
	{
		fontPath += "Medium";
		break;
	}
	case FONT_BOLD:
	{
		fontPath += "Bold";
		break;
	}
	}

	if (m_italic)
	{
		fontPath += "Italic";
	}

	fontPath += ".ttf";

	m_font = TTF_OpenFont(fontPath.c_str(), m_pointSize);

	if(!m_font)
	{
		m_font = oldFont;
		success = false;
	}

	return success;
}

//=============================================================================
// Function: void closeFont()
// Description:
// Closes a font if it currently exists.
//=============================================================================
void Font::closeFont()
{
	if(m_font)
	{
		TTF_CloseFont(m_font);
	}
}