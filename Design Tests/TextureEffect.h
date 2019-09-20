#pragma once
//==========================================================================================
// File Name: TextureEffect.h
// Author: Brian Blackmon
// Date Created: 6/29/2019
// Purpose: 
// Holds information for creating an effect for a texture.
//==========================================================================================
#include "Timer.h"
#include <SDL.h>

class SpriteComponent;

class TextureEffect
{
public:
	enum EffectType
	{
		EFFECT_COLOR_FADE,
		EFFECT_COLOR_FLASH,
		EFFECT_ALPHA_FADE,
		EFFECT_ALPHA_FLASH,
		EFFECT_ALL_FADE,
		EFFECT_ALL_FLASH
	};

	TextureEffect(SpriteComponent *sprite, 
		EffectType type, 
		SDL_Color endColor, 
		SDL_BlendMode blendMode, 
		float duration,
		float speed);

	~TextureEffect();

	void start();
	void applyEffect(float delta);
	void reset();

	void setEndColor(SDL_Color endColor) { m_endColor = endColor; }
	void setBlendMode(SDL_BlendMode blendMode) { m_blendMode = blendMode; }
	void setDuration(float duration) { m_duration = duration; }
	void setSpeed(float speed) { m_speed = speed; }
	void setPaused(bool paused) { m_paused = paused; }

	EffectType getType() { return m_type; }
	SDL_Color getCurrentColor() { return m_currentColor; }
	SDL_BlendMode getBlendMode() { return m_blendMode; }
	float getSpeed() { return m_speed; }
	float getDuration() { return m_duration; }
	bool getStarted() { return m_started; }
	bool getPaused() { return m_paused; }

protected:
	SpriteComponent *m_sprite;
	SDL_BlendMode m_blendMode;
	SDL_Color m_startColor;
	SDL_Color m_endColor;
	SDL_Color m_currentColor;

	EffectType m_type;

	float m_currentTime;
	float m_speed;
	float m_duration;
	bool m_started;
	bool m_colorFadeComplete;
	bool m_alphaFadeComplete;
	bool m_paused;

	float lerp(int a, int b, float percent);

	void colorFade();
	void colorFlash();
	void alphaFade();
	void alphaFlash();


};