#include "TextComponent.h"
#include <SDL_ttf.h>
#include "Font.h"
#include "Texture.h"
#include "ResourceManager.h"

TextComponent::TextComponent(Font *font,
	SDL_Color color,
	string text,
	Uint32 wrapWidth,
	Vector2D position)
	:Component(TEXT),
	m_font(font),
	m_text(text),
	m_color(color),
	m_wrapWidth(wrapWidth),
	m_position(position),
	m_texture(NULL),
	m_visible(true)
{
	createTexture();
}

TextComponent::~TextComponent()
{
	m_font = NULL;

	if(m_texture)
	{
		destroyTexture();
	}
}

//=============================================================================
// Function: void setText(string)
// Description:
// Sets the text to display.
// Parameters:
// string text - The new text to display.
//=============================================================================
void TextComponent::setText(string text)
{
	if(m_text != text)
	{
		m_text = text;
		
		createTexture();
	}
}

//=============================================================================
// Function: void setColor(SDL_Color)
// Description:
// Sets the color of the text.
// Parameters:
// SDL_Color color - The color to change the text to.
//=============================================================================
void TextComponent::setColor(SDL_Color color)
{
	m_color = color;

	createTexture();
}

//=============================================================================
// Function: void setColor(int, int, int, int)
// Description:
// Sets the color to the specified values.
// Parameters:
// int r - The red amount.
// int g - The green amount.
// int b - The blue amount.
// int a - The alpha amount.
//=============================================================================
void TextComponent::setColor(int r, int g, int b, int a)
{
	const int MAX_VALUE = 255;

	if (r < 0) { r = 0; }
	else if (MAX_VALUE < r) { r = MAX_VALUE; }

	if (g < 0) { g = 0; }
	else if (MAX_VALUE < g) { g = MAX_VALUE; }

	if (b < 0) { b = 0; }
	else if (MAX_VALUE < b) { b = MAX_VALUE; }

	if (a < 0) { a = 0; }
	else if (MAX_VALUE < a) { a = MAX_VALUE; }

	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;

	createTexture();
}

//=============================================================================
// Function: void setFont(Font*)
// Description:
// Changes the font to the specified font.
// Parameters:
// Font *font - The font to change to.
//=============================================================================
void TextComponent::setFont(Font *font)
{
	m_font = font;

	createTexture();
}

//=============================================================================
// Function: void setWrapWidth(Uint32)
// Description:
// Sets the wrapping width of the text.
// If set to 0, the text doesn't wrap.
// Parameters:
// Uint32 width - The wrapping width in pixels.
//=============================================================================
void TextComponent::setWrapWidth(Uint32 width)
{
	if (width < 0) { width = 0; }

	m_wrapWidth = width;

	createTexture();
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the text.
// Parameters:
// int width - The new width of the text.
//=============================================================================
void TextComponent::setWidth(int width)
{
	m_width = width;

	setWrapWidth(width);
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the text.
// Parameters:
// int height - The new height of the text.
//=============================================================================
void TextComponent::setHeight(int height)
{
	m_height = height;

	createTexture();
}

//=============================================================================
// Function: void createTexture()
// Description:
// Creates a text texture based on the current text information.
//=============================================================================
void TextComponent::createTexture()
{
	if (m_font)
	{
		if (m_texture)
		{
			destroyTexture();
		}

		fixTextSize();

		SDL_Surface *textSurface = NULL;

		if (0 < m_wrapWidth)
		{
			textSurface = TTF_RenderText_Blended_Wrapped(m_font->getFont(), m_text.c_str(), m_color, m_wrapWidth);
		}
		else
		{
			textSurface = TTF_RenderText_Blended(m_font->getFont(), m_text.c_str(), m_color);
		}

		if (textSurface)
		{
			SDL_Texture *temp = SDL_CreateTextureFromSurface(ResourceManager::instance()->renderer()->renderer(), textSurface);

			if (temp)
			{
				Vector2D position{ (float)(textSurface->w / 2), (float)(textSurface->h / 2) };

				m_texture = new Texture(temp, position);
			}

			SDL_FreeSurface(textSurface);
		}
	}
}

//=============================================================================
// Function: void destroyTexture()
// Description:
// Destroys the current texture if it exists.
//=============================================================================
void TextComponent::destroyTexture()
{
	if(m_texture)
	{
		delete m_texture;
		m_texture = NULL;
	}
}

//=============================================================================
// Function: void fixTextSize()
// Description:
// Fixes the font size of the text to make sure that it fits within
// the width and height of the text object.
//=============================================================================
void TextComponent::fixTextSize()
{
	const int MIN_PT = 2;
	const int MAX_PT = 72;

	int fontSize = m_font->getPointSize();
	// Adjust the font by the width before the height.
	while (!wordsFitWidth() && MIN_PT < fontSize)
	{
		fontSize--;
		m_font->setPointSize(fontSize);
	}

	int textWidth = 0;
	int textLineHeight = 0;

	m_font->getTextSize(m_text, &textWidth, &textLineHeight);

	int textHeight = textLineHeight;

	if (0 < m_wrapWidth)
	{
		textHeight = (textWidth / m_wrapWidth) * textLineHeight;
	}

	if (m_height < textHeight)
	{
		while (m_height < textHeight && MIN_PT < fontSize)
		{
			fontSize--;
			m_font->setPointSize(fontSize);

			m_font->getTextSize(m_text, &textWidth, &textLineHeight);

			if (0 < m_wrapWidth)
			{
				textHeight = (textWidth / m_wrapWidth) * textLineHeight;
			}
			else
			{
				textHeight = textLineHeight;
			}
		}
	}
	else if (textHeight < m_height)
	{
		while (textHeight < m_height &&
			wordsFitWidth() &&
			fontSize < MAX_PT)
		{
			fontSize++;
			m_font->setPointSize(fontSize);

			m_font->getTextSize(m_text, &textWidth, &textLineHeight);

			if (0 < m_wrapWidth)
			{
				textHeight = (textWidth / m_wrapWidth) * textLineHeight;
			}
			else
			{
				textHeight = textLineHeight;
			}
		}

		m_font->setPointSize(fontSize - 1);
	}
}

//=============================================================================
// Function: bool wordsFitWidth()
// Description:
// Checks to see if all of the words in the text fit the current width.
// Output:
// bool
// Returns true if all words are less than the width.
// Returns false if any word is greater than the width.
//=============================================================================
bool TextComponent::wordsFitWidth()
{
	bool wordsFit = true;

	std::string currentWord = "";

	for (int i = 0; i < m_text.length(); i++)
	{
		if (m_text[i] == ' ' ||
			m_text[i] == '\0')
		{
			int wordWidth = getWordWidth(currentWord);

			if (m_width <= wordWidth)
			{
				wordsFit = false;
				break;
			}

			currentWord = "";
		}
		else
		{
			currentWord += m_text[i];
		}
	}

	return wordsFit;
}

//=============================================================================
// Function: int getWordWidth(std::string word)
// Description:
// Gets the total render width of the specified string.
// Parameters:
// string word - The word to get the width of.
// Output:
// int
// Returns the width of all of the characters in the string.
//=============================================================================
int TextComponent::getWordWidth(std::string word)
{
	int width = 0;

	for (int i = 0; i < word.length(); i++)
	{
		if (word[i] != '\0')
		{
			int glyphWidth = 0;

			if (m_font)
			{
				if (m_font->getGlyphMetrics(word[i], NULL, NULL, NULL, NULL, &glyphWidth) != -1)
				{
					width += glyphWidth;
				}
			}
		}
	}

	return width;
}