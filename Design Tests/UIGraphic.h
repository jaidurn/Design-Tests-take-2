#pragma once
//==========================================================================================
// File Name: UIGraphic.h
// Author: Brian Blackmon
// Date Created: 9/11/2019
// Purpose: 
// Extends the UIComponent class and adds functionality for graphical UI.
//==========================================================================================
#include "UIComponent.h"
#include "RenderSystem.h"

class UIGraphic : public UIComponent
{
public:
	UIGraphic(int entityID, Vector2D position, string name);
	virtual ~UIGraphic();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setVisible(bool visible);
	virtual void setActive(bool active);

	void setVisualEffect(TextureEffect::EffectType effect, 
		SDL_Color color, 
		float duration, 
		float speed);
	void stopVisualEffect();
	
	void setLayer(RenderSystem::RenderLayers layer);
};

