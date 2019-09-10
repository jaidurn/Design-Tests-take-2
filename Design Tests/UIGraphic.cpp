#include "UIGraphic.h"
#include "RenderSystem.h"
#include "MessageSystem.h"
#include "MoveMessage.h"

UIGraphic::UIGraphic(int entityID, 
	Vector2D position,
	string graphicPath)
	:UIComponent(UI_GRAPHIC, entityID, position),
	m_rect(NULL)
{
	SpriteComponent *sprite = RenderSystem::instance()->createSprite(entityID, graphicPath, position);

	if(sprite)
	{
		RenderSystem::instance()->setSpriteLayer(entityID, RenderSystem::RENDER_UI_FOREGROUND);

		m_rect = new Rectangle(
			position.getX(), 
			position.getY(),
			sprite->width(),
			sprite->height());
	}
}

UIGraphic::~UIGraphic()
{

}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the position of the graphic UI.
// Parameters:
// Vector2D position - The new position for the graphic.
//=============================================================================
void UIGraphic::setPosition(Vector2D position)
{
	MoveMessage *move = new MoveMessage(m_entityID, m_position, position);

	MessageSystem::instance()->pushMessage(move);

	m_position = position;
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the graphic.
// Parameters:
// int width - The new width of the graphic.
//=============================================================================
void UIGraphic::setWidth(int width)
{
	if (0 < width)
	{
		m_rect->setWidth(width);

		SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

		if (sprite)
		{
			SDL_Rect clip = sprite->clip();

			sprite->setClip(clip.x, clip.y, m_rect->width(), clip.h);
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the graphic.
// Parameters:
// int height - The new height of the graphic.
//=============================================================================
void UIGraphic::setHeight(int height)
{
	if (0 < height)
	{
		m_rect->setHeight(height);

		SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

		if (sprite)
		{
			SDL_Rect clip = sprite->clip();

			sprite->setClip(clip.x, clip.y, clip.w, m_rect->height());
		}
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visiblity of the graphic.
// Parameters:
// bool visible - The visibility of the graphic.
//=============================================================================
void UIGraphic::setVisible(bool visible)
{
	SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

	if(sprite)
	{
		sprite->setVisible(visible);
	}

	m_visible = visible;
}

//=============================================================================
// Function: void setClip(SDL_Rect clip)
// Description:
// Sets the clip for rendering the graphic.
// Parameters:
// SDL_Rect clip - The clip to use when rendering the graphic.
//=============================================================================
void UIGraphic::setClip(SDL_Rect clip)
{
	SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

	if(sprite)
	{
		sprite->setClip(clip.x, clip.y, clip.w, clip.h);
	}
}

//=============================================================================
// Function: void setRect(int width, int height)
// Description:
// Sets the size of the graphic object and sets the render size
// of the sprite tied to it.
// Parameters:
// int width - The width of the rect.
// int height - The height of the rect.
//=============================================================================
void UIGraphic::setRect(int width, int height)
{
	if(m_rect)
	{
		if (0 < width) { m_rect->setWidth(width); }
		if (0 < height) { m_rect->setHeight(height); }

		SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

		if(sprite)
		{
			sprite->setRenderSize(m_rect->width(), m_rect->height());
		}
	}
}

//=============================================================================
// Function: void update(float)
// Description:
// Updates the graphic.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void UIGraphic::update(float delta)
{

}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes the messages passed to it.
// Parameters:
// IMessage *message - The message to process. 
//=============================================================================
void UIGraphic::processMessage(IMessage *message)
{

}
