#pragma once
//==========================================================================================
// File Name: Font.h
// Author: Brian Blackmon
// Date Created: 8/20/2019
// Purpose: 
// A wrapping class for the TTF_Font struct.
//==========================================================================================
#include <SDL_ttf.h>
#include <string>

typedef std::string string;

class Font
{
public:
	enum FontFlags
	{
		FONT_THIN,
		FONT_LIGHT,
		FONT_REGULAR,
		FONT_MEDIUM,
		FONT_BOLD,
	};

	Font(string fontPath, int pointSize, FontFlags fontFlag, bool italic);
	~Font();

	TTF_Font *getFont() { return m_font; }

	bool isBold();
	bool isItalic();
	bool isGlyphProvided(Uint16 glyph);

	int getOutline();
	int getHeight();
	int getAscent();
	int getDescent();
	int getLineSkip();

	int getGlyphMetrics(Uint16 glyph, int *minX, int *minY, int *maxX, int *maxY, int *advance);
	int getTextSize(string text, int *width, int *height);
	int getPointSize() { return m_pointSize; }

	void setItalic(bool italic);
	void setFlag(FontFlags flag);
	void setPointSize(int pointSize);

private:
	TTF_Font *m_font;

	string m_fontPath;
	int m_pointSize;
	bool m_italic;
	FontFlags m_flag;

	bool createFont();
	void closeFont();
};

