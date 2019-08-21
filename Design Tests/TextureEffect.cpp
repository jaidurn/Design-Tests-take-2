#include "TextureEffect.h"
#include "SpriteComponent.h"
#include <iostream>

TextureEffect::TextureEffect(SpriteComponent *sprite, EffectType type, SDL_Color endColor, SDL_BlendMode blendMode, float duration, float speed)
	:m_sprite(sprite), m_type(type), m_endColor(endColor), m_colorFadeComplete(false), m_alphaFadeComplete(false), m_duration(duration), m_speed(speed), m_currentTime(0.0f), m_blendMode(blendMode), m_started(false)
{
	m_startColor.r = 0;
	m_startColor.g = 0;
	m_startColor.b = 0;
	m_startColor.a = 0;

	m_currentColor.r = 0;
	m_currentColor.g = 0;
	m_currentColor.b = 0;
	m_currentColor.a = 0;

	if (m_sprite)
	{
		m_startColor = m_sprite->colorMod();
		m_currentColor = m_sprite->colorMod();
	}
}

TextureEffect::~TextureEffect()
{

}

void TextureEffect::start()
{
	if(!started())
	{
		m_currentTime = 0.0f;
		m_started = true;
	}
}

void TextureEffect::applyEffect(float delta)
{
	if(m_currentTime < m_duration)
	{
		switch(m_type)
		{
		case EFFECT_COLOR_FADE:
		{
			colorFade();
			break;
		}
		case EFFECT_COLOR_FLASH:
		{
			colorFlash();
			break;
		}
		case EFFECT_ALPHA_FADE:
		{
			alphaFade();
			break;
		}
		case EFFECT_ALPHA_FLASH:
		{
			alphaFlash();
			break;
		}
		case EFFECT_ALL_FADE:
		{
			colorFade();
			alphaFade();
			break;
		}
		case EFFECT_ALL_FLASH:
		{
			colorFlash();
			alphaFlash();
			break;
		}
		}

		m_currentTime += delta;
	}
	else
	{
		switch(m_type)
		{
		case EFFECT_COLOR_FADE:
		{
			m_currentColor.r = m_endColor.r;
			m_currentColor.g = m_endColor.g;
			m_currentColor.b = m_endColor.b;

			break;
		}
		case EFFECT_COLOR_FLASH:
		{
			if(!m_colorFadeComplete)
			{
				m_currentColor.r = m_endColor.r;
				m_currentColor.g = m_endColor.g;
				m_currentColor.b = m_endColor.b;
			}
			else
			{
				m_currentColor.r = m_startColor.r;
				m_currentColor.g = m_startColor.g;
				m_currentColor.b = m_startColor.b;
			}

			break;
		}
		case EFFECT_ALPHA_FADE:
		{
			m_currentColor.a = m_endColor.a;
			break;
		}
		case EFFECT_ALPHA_FLASH:
		{
			if(!m_alphaFadeComplete)
			{
				m_currentColor.a = m_endColor.a;
			}
			else
			{
				m_currentColor.a = m_startColor.a;
			}

			break;
		}
		case EFFECT_ALL_FADE:
		{
			m_currentColor = m_endColor;
			break;
		}
		case EFFECT_ALL_FLASH:
		{
			if (!m_alphaFadeComplete)
			{
				m_currentColor.a = m_endColor.a;
			}
			else
			{
				m_currentColor.a = m_startColor.a;
			}

			if (!m_colorFadeComplete)
			{
				m_currentColor.r = m_endColor.r;
				m_currentColor.g = m_endColor.g;
				m_currentColor.b = m_endColor.b;
			}
			else
			{
				m_currentColor.r = m_startColor.r;
				m_currentColor.g = m_startColor.g;
				m_currentColor.b = m_startColor.b;
			}

			break;
		}
		}
	}
}

void TextureEffect::reset()
{
	m_currentTime = 0.0f;

	m_colorFadeComplete = false;
	m_alphaFadeComplete = false;

	m_currentColor = m_startColor;
	m_started = false;

}

float TextureEffect::lerp(int a, int b, float percent)
{
/*	if (0.98 < percent)
	{
		percent = 1;
	}
*/
	float amount = ((1.0f - percent) * (float)a) + (percent * (float)b);

	return amount;
}


//=============================================================================
// Function: void colorFade()
// Description:
// Fades the color from the starting color to the ending color.
//=============================================================================
void TextureEffect::colorFade()
{
	float testTime = (m_currentTime / m_duration);

	m_currentColor.r = (Uint8)round(lerp(m_startColor.r, m_endColor.r, testTime));
	m_currentColor.g = (Uint8)round(lerp(m_startColor.g, m_endColor.g, testTime));
	m_currentColor.b = (Uint8)round(lerp(m_startColor.b, m_endColor.b, testTime));

	m_sprite->setColorMod(m_currentColor);
}

//=============================================================================
// Function: void colorFlash()
// Description:
// Flashes the color between the starting color and the ending color.
//=============================================================================
void TextureEffect::colorFlash()
{
	float durationSpeed = m_duration / m_speed;
	float testTime = (fmod(m_currentTime, durationSpeed) / durationSpeed);

	static float prevTime = 0;

	if (!m_colorFadeComplete)
	{
		//Check and see if the lerp needs to be added to the current color instead.

		if (prevTime < testTime)
		{
			if (m_currentColor.r != m_startColor.r)
			{
				m_currentColor.r = m_endColor.r;
			}

			if (m_currentColor.g != m_startColor.g)
			{
				m_currentColor.g = m_endColor.g;
			}

			if (m_currentColor.b != m_startColor.b)
			{
				m_currentColor.b = m_endColor.b;
			}
		}

		if (m_currentColor.r != m_endColor.r)
		{
			m_currentColor.r = (Uint8)round(lerp(m_startColor.r, m_endColor.r, testTime));
		}

		if (m_currentColor.g != m_endColor.g)
		{
			m_currentColor.g = (Uint8)round(lerp(m_startColor.g, m_endColor.g, testTime));
		}

		if (m_currentColor.b != m_endColor.b)
		{
			m_currentColor.b = (Uint8)round(lerp(m_startColor.b, m_endColor.b, testTime));
		}

		if (m_startColor.r < m_endColor.r)
		{
			if (m_endColor.r < m_currentColor.r)
			{
				m_currentColor.r = m_endColor.r;
			}
		}
		else if (m_endColor.r < m_startColor.r)
		{
			if (m_currentColor.r < m_endColor.r)
			{
				m_currentColor.r = m_endColor.a;
			}
		}

		if (m_startColor.g < m_endColor.g)
		{
			if (m_endColor.g < m_currentColor.g)
			{
				m_currentColor.g = m_endColor.g;
			}
		}
		else if (m_endColor.g < m_startColor.g)
		{
			if (m_currentColor.g < m_endColor.g)
			{
				m_currentColor.g = m_endColor.g;
			}
		}

		if (m_startColor.r < m_endColor.r)
		{
			if (m_endColor.r < m_currentColor.r)
			{
				m_currentColor.r = m_endColor.r;
			}
		}
		else if (m_endColor.b < m_startColor.b)
		{
			if (m_currentColor.b < m_endColor.b)
			{
				m_currentColor.b = m_endColor.b;
			}
		}

		if (m_currentColor.r == m_endColor.r &&
			m_currentColor.g == m_endColor.g &&
			m_currentColor.b == m_endColor.b)
		{
			m_colorFadeComplete = true;
		}
	}
	else if (m_colorFadeComplete)
	{
		//Check and see if the lerp needs to be added to the current color instead.

		if (testTime < prevTime)
		{
			if (m_currentColor.r != m_endColor.r)
			{
				m_currentColor.r = m_startColor.r;
			}

			if (m_currentColor.g != m_endColor.g)
			{
				m_currentColor.g = m_startColor.g;
			}

			if (m_currentColor.b != m_endColor.b)
			{
				m_currentColor.b = m_startColor.b;
			}
		}

		if (m_currentColor.r != m_startColor.r)
		{
			m_currentColor.r = (Uint8)round(lerp(m_endColor.r, m_startColor.r, testTime));
		}

		if (m_currentColor.g != m_startColor.g)
		{
			m_currentColor.g = (Uint8)round(lerp(m_endColor.g, m_startColor.g, testTime));
		}

		if (m_currentColor.b != m_startColor.b)
		{
			m_currentColor.b = (Uint8)round(lerp(m_endColor.b, m_startColor.b, testTime));
		}

		if (m_endColor.r < m_startColor.r)
		{
			if (m_startColor.r < m_currentColor.r)
			{
				m_currentColor.r = m_startColor.r;
			}
		}
		else if (m_startColor.r < m_endColor.r)
		{
			if (m_currentColor.r < m_startColor.r)
			{
				m_currentColor.r = m_startColor.a;
			}
		}

		if (m_endColor.g < m_startColor.g)
		{
			if (m_startColor.g < m_currentColor.g)
			{
				m_currentColor.g = m_startColor.g;
			}
		}
		else if (m_startColor.g < m_endColor.g)
		{
			if (m_currentColor.g < m_startColor.g)
			{
				m_currentColor.g = m_startColor.g;
			}
		}

		if (m_endColor.r < m_startColor.r)
		{
			if (m_startColor.r < m_currentColor.r)
			{
				m_currentColor.r = m_startColor.r;
			}
		}
		else if (m_startColor.b < m_endColor.b)
		{
			if (m_currentColor.b < m_startColor.b)
			{
				m_currentColor.b = m_startColor.b;
			}
		}



		if (m_currentColor.r == m_startColor.r && 
			m_currentColor.g == m_startColor.g &&
			m_currentColor.b == m_startColor.b)
		{
			m_colorFadeComplete = false;
		}
	}

	m_sprite->setColorMod(m_currentColor);

	prevTime = testTime;
}

//=============================================================================
// Function: void alphaFade()
// Description:
// Fades the alpha from the starting to ending alpha.
//=============================================================================
void TextureEffect::alphaFade()
{
	float testTime = (m_currentTime / m_duration);

	m_currentColor.a = (Uint8)round(lerp(m_startColor.a, m_endColor.a, testTime));
	
	m_sprite->setColorMod(m_currentColor);
}

//=============================================================================
// Function: void alphaFlash()
// Description:
// Flashes the alpha between the starting alpha and ending alpha.
//=============================================================================
void TextureEffect::alphaFlash()
{
	float durationSpeed = m_duration / m_speed;
	float testTime = (fmod(m_currentTime, durationSpeed) / durationSpeed);
	int colorChange = m_currentColor.a;

	static float prevTime = 0;

	if(!m_alphaFadeComplete)
	{
		if(testTime < prevTime && colorChange != m_startColor.a)
		{
			colorChange = m_endColor.a;
		}

		if(colorChange != m_endColor.a)
		{
			colorChange = (int)round(lerp(m_startColor.a, m_endColor.a, testTime));
		}

		if(colorChange == m_endColor.a)
		{
			m_alphaFadeComplete = true;
		}
		else if(m_startColor.a < m_endColor.a)
		{
			if(m_endColor.a <= colorChange)
			{
				m_alphaFadeComplete = true;
				colorChange = m_endColor.a;
			}
		}
		else if(m_endColor.a < m_startColor.a)
		{
			if(colorChange <= m_endColor.a)
			{
				m_alphaFadeComplete = true;
				colorChange = m_endColor.a;
			}
		}
	}
	else if(m_alphaFadeComplete)
	{
		//Check and see if the lerp needs to be added to the current color instead.
		if(testTime < prevTime && colorChange != m_endColor.a)
		{
			colorChange = m_startColor.a;
		}

		if(colorChange != m_startColor.a)
		{
			colorChange = (int)round(lerp(m_endColor.a, m_startColor.a, testTime));
		}

		if (colorChange == m_startColor.a)
		{
			m_alphaFadeComplete = false;
		}
		else if (m_endColor.a < m_startColor.a)
		{
			if (m_startColor.a < colorChange)
			{
				m_alphaFadeComplete = false;
				colorChange = m_startColor.a;
			}
		}
		else if (m_startColor.a < m_endColor.a)
		{
			if (colorChange < m_startColor.a)
			{
				m_alphaFadeComplete = false;
				colorChange = m_startColor.a;
			}
		}
	}

	m_currentColor.a = (Uint8)colorChange;

	prevTime = testTime;
}