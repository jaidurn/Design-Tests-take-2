#include "UIText.h"
#include "RenderSystem.h"
#include "TextComponent.h"
#include "Font.h"
#include "MessageSystem.h"
#include "EntityDestroyMessage.h"

UIText::UIText(int entityID, Vector2D position, string name)
	:UIComponent(entityID, UI_TEXT, position, name)
{

}

UIText::~UIText()
{
	EntityDestroyMessage *destroy = new EntityDestroyMessage(m_entityID);

	MessageSystem::instance()->pushMessage(destroy);
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the position of the text and related components.
// Parameters:
// Vector2D position - The new position of the text.
//=============================================================================
void UIText::setPosition(Vector2D position)
{
	UIComponent::setPosition(position);
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the text and the related components.
// Parameters:
// int width - The new width of the text.
//=============================================================================
void UIText::setWidth(int width)
{
	if (m_rect)
	{
		m_rect->setWidth(width);

		RenderSystem *renderSys = RenderSystem::instance();

		TextComponent *textComp = renderSys->getText(m_entityID);

		if (textComp)
		{
			textComp->setWidth(width);
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the text and related components.
// Parameters:
// int height - The new height of the text.
//=============================================================================
void UIText::setHeight(int height)
{
	if (m_rect)
	{
		m_rect->setHeight(height);

		RenderSystem *renderSys = RenderSystem::instance();

		TextComponent *textComp = renderSys->getText(m_entityID);

		if (textComp)
		{
			textComp->setHeight(height);
		}
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visible state for the text and related components.
// Parameters:
// bool visible - The new visible state.
//=============================================================================
void UIText::setVisible(bool visible)
{
	if (m_visible != visible)
	{
		m_visible = visible;

		RenderSystem *renderSys = RenderSystem::instance();

		TextComponent *textComp = renderSys->getText(m_entityID);

		if (textComp)
		{
			textComp->setVisible(m_visible);
		}
	}
}

//=============================================================================
// Function: void setFont(Font*)
// Description:
// Sets the font of the text.
// Parameters:
// Font *font - The new font.
//=============================================================================
void UIText::setFont(Font *font)
{
	RenderSystem *renderSys = RenderSystem::instance();

	TextComponent *textComp = renderSys->getText(m_entityID);

	if (textComp)
	{
		textComp->setFont(font);
	}
}

//=============================================================================
// Function: void setText(string)
// Description:
// Sets the text of the component.
// Parameters:
// string text - The new text.
//=============================================================================
void UIText::setText(string text)
{
	RenderSystem *renderSys = RenderSystem::instance();

	TextComponent *textComp = renderSys->getText(m_entityID);

	if (textComp)
	{
		textComp->setText(text);
	}
}

//=============================================================================
// Function: void setColor(SDL_Color)
// Description:
// Sets the color of the text and related components.
// Parameters:
// SDL_Color color - The new color of the text.
//=============================================================================
void UIText::setColor(SDL_Color color)
{
	RenderSystem *renderSys = RenderSystem::instance();

	TextComponent *textComp = renderSys->getText(m_entityID);

	if (textComp)
	{
		textComp->setColor(color);
	}
}