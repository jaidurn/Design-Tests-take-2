#include "UIText.h"
#include "RenderSystem.h"
#include "MessageSystem.h"
#include "MoveMessage.h"

UIText::UIText(int entityID,
	Vector2D position, 
	string text,
	SDL_Color color,
	Font *font)
	:UIComponent(UI_TEXT, entityID, position)
{
	TextComponent *textComp = RenderSystem::instance()->createTextComponent(entityID, text, font, color, 0, position);
}


UIText::~UIText()
{

}

//=============================================================================
// Function: void setFont(Font*)
// Description:
// Sets the font for the text object.
// Parameters:
// Font *font - The font to use for the text.
//=============================================================================
void UIText::setFont(Font *font)
{
	TextComponent *textComp = RenderSystem::instance()->getText(m_entityID);

	if(textComp)
	{
		textComp->setFont(font);
	}
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the text UI.
// Parameters:
// int width - The width to set the text to.
//=============================================================================
void UIText::setWidth(int width)
{
	this->setWrapWidth(width);
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the text.
// Parameters:
// int height - The height to set the text to.
//=============================================================================
void UIText::setHeight(int height)
{
	// TODO: Figure out if you want to clip text textures.
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the position of the UIText component.
// Parameters:
// Vector2D position - The position to move the component to.
//=============================================================================
void UIText::setPosition(Vector2D position)
{
	MoveMessage *move = new MoveMessage(m_entityID, m_position, position);

	MessageSystem::instance()->pushMessage(move);

	m_position = position;
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visiblity of the text.
// Parameters:
// bool visible - The visibility of the text.
//=============================================================================
void UIText::setVisible(bool visible)
{
	TextComponent *textComp = RenderSystem::instance()->getText(m_entityID);

	if(textComp)
	{
		textComp->setVisible(visible);
	}

	m_visible = visible;
}

//=============================================================================
// Function: void setText(string)
// Description:
// Sets the text of the UIText component.
// Parameters:
// string text - The text to change to.
//=============================================================================
void UIText::setText(string text)
{
	TextComponent *textComp = RenderSystem::instance()->getText(m_entityID);

	if(textComp)
	{
		textComp->setText(text);
	}
}

//=============================================================================
// Function: void setColor(SDL_Color)
// Description:
// Sets the rendering color of the text.
// Parameters:
// SDL_Color color - The color to set the text to.
//=============================================================================
void UIText::setColor(SDL_Color color)
{
	TextComponent *textComp = RenderSystem::instance()->getText(m_entityID);

	if(textComp)
	{
		textComp->setColor(color);
	}
}

//=============================================================================
// Function: void setWrapWidth(Uint32)
// Description:
// Sets the wrap width of the text.
// Parameters:
// Uint32 wrapWidth - How wide the text should be in pixels before
// wrapping around to a new line.
//=============================================================================
void UIText::setWrapWidth(Uint32 wrapWidth)
{
	TextComponent *textComp = RenderSystem::instance()->getText(m_entityID);

	if(textComp)
	{
		textComp->setWrapWidth(wrapWidth);
	}
}

//=============================================================================
// Function: void update(float)
// Description:
// Updates the component.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void UIText::update(float delta)
{

}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes any messages passed to it.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIText::processMessage(IMessage *message)
{

}