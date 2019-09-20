#include "UIGraphic.h"
#include "RenderSystem.h"
#include "SpriteComponent.h"
#include "MessageSystem.h"
#include "EntityDestroyMessage.h"

UIGraphic::UIGraphic(int entityID,
	Vector2D position,
	string name)
	:UIComponent(entityID, UI_GRAPHIC, position, name)
{

}

UIGraphic::~UIGraphic()
{
	EntityDestroyMessage *destroy = new EntityDestroyMessage(m_entityID);

	MessageSystem::instance()->pushMessage(destroy);
}

//=============================================================================
// Function: void setPosition(Vector2D position)
// Description:
// Sets the position of the graphic and anything related to it.
// Parameters:
// Vector2D position - The new position of the component.
//=============================================================================
void UIGraphic::setPosition(Vector2D position)
{
	UIComponent::setPosition(position);
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the graphic and the size of the sprite
// attatched.
// Parameters:
// int width - The new width of the graphic.
//=============================================================================
void UIGraphic::setWidth(int width)
{
	if (m_rect)
	{
		m_rect->setWidth(width);

		RenderSystem *renderSys = RenderSystem::instance();

		SpriteComponent *sprite = renderSys->getSprite(m_entityID);

		if (sprite)
		{
			SDL_Rect renderSize = sprite->renderSize();

			sprite->setRenderSize(width, renderSize.h);
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the graphic and related components.
// Parameters:
// int height - The new height of the graphic.
//=============================================================================
void UIGraphic::setHeight(int height)
{
	if (m_rect)
	{
		m_rect->setHeight(height);

		RenderSystem *renderSys = RenderSystem::instance();

		SpriteComponent *sprite = renderSys->getSprite(m_entityID);

		if (sprite)
		{
			SDL_Rect renderSize = sprite->renderSize();

			sprite->setRenderSize(renderSize.w, height);
		}
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visibilty of the graphic and related components.
// Parameters:
// bool visible - The new visible state to set.
//=============================================================================
void UIGraphic::setVisible(bool visible)
{
	if (m_visible != visible)
	{
		RenderSystem *renderSys = RenderSystem::instance();

		SpriteComponent *sprite = renderSys->getSprite(m_entityID);
		AnimationComponent *animation = renderSys->getAnimation(m_entityID);
		TextureEffect *textureEffect = renderSys->getEffect(m_entityID);

		if (sprite)
		{
			sprite->setVisible(visible);
		}

		if (!m_visible && m_active)
		{
			if (animation)
			{
				animation->setPaused(false);
			}

			if (textureEffect)
			{
				textureEffect->setPaused(false);
			}
		}
		else
		{
			if (animation)
			{
				animation->setPaused(true);
			}
			
			if (textureEffect)
			{
				textureEffect->setPaused(true);
			}
		}

		m_visible = visible;
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state of the graphic and related components.
// Parameters:
// bool active - The new active state.
//=============================================================================
void UIGraphic::setActive(bool active)
{
	if (m_active != active)
	{
		RenderSystem *renderSys = RenderSystem::instance();

		AnimationComponent *animation = renderSys->getAnimation(m_entityID);
		TextureEffect *effect = renderSys->getEffect(m_entityID);

		if (!m_active && m_visible)
		{
			// Effects and animations can be enabled!

			if (animation)
			{
				animation->setPaused(false);
			}

			if (effect)
			{
				effect->setPaused(false);
			}
		}
		else
		{
			// Effects and animations are disabled!
			if (animation)
			{
				animation->setPaused(true);
			}

			if (effect)
			{
				effect->setPaused(true);
			}
		}

		m_active = active;
	}
}

//=============================================================================
// Function: void setLayer(RenderLayers)
// Description:
// Sets the rendering layer for our graphic.
// Parameters:
// RenderLayers layer - The new render layer for our graphic.
//=============================================================================
void UIGraphic::setLayer(RenderSystem::RenderLayers layer)
{
	if (RenderSystem::RENDER_UI_BACKGROUND <= layer &&
		layer <= RenderSystem::RENDER_UI_FOREGROUND)
	{
		RenderSystem *renderSys = RenderSystem::instance();

		renderSys->setSpriteLayer(m_entityID, layer);
	}
}