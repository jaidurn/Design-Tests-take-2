#include "Renderer.h"
#include "ErrorSystem.h"
#include "Vector2D.h"
#include "Rotation.h"
#include "Texture.h"
#include <iostream>
#include <cmath>

Renderer::Renderer(SDL_Window *window)
{
	m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if(!m_renderer)
	{
		ErrorSystem::logError("Failed to create renderer!");
		this->~Renderer();
	}
	else
	{
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	}
}

Renderer::Renderer(SDL_Window *window, bool vSyncEnabled)
{
	SDL_RendererFlags flag = SDL_RENDERER_ACCELERATED;

	if(vSyncEnabled)
	{
		flag = SDL_RENDERER_PRESENTVSYNC;
	}

	m_renderer = SDL_CreateRenderer(window, -1, flag);

	if(!m_renderer)
	{
		ErrorSystem::logError("Failed to create renderer!");
		this->~Renderer();
	}
	else
	{
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	}
}

Renderer::~Renderer()
{
	if (m_renderer) { SDL_DestroyRenderer(m_renderer); }
}

bool Renderer::hasPixelFormat(Uint32 format)
{
	bool formatFound = false;

	SDL_RendererInfo info;

	SDL_GetRendererInfo(m_renderer, &info);

	for(unsigned int i = 0; i < info.num_texture_formats; i++)
	{
		if(info.texture_formats[i] == format)
		{
			formatFound = true;
			break;
		}
	}

	return formatFound;
}

Uint32 Renderer::getAlphaPixelFormat()
{
	SDL_RendererInfo info;
	SDL_GetRendererInfo(m_renderer, &info);

	for(Uint32 i = 0; i < info.num_texture_formats; i++)
	{
		if(SDL_ISPIXELFORMAT_ALPHA(info.texture_formats[i]) != 0)
		{
			return info.texture_formats[i];
		}
	}

	return -1;
}

void Renderer::renderPresent()
{
	SDL_RenderPresent(m_renderer);
}

void Renderer::clear()
{
	SDL_RenderClear(m_renderer);
}

void Renderer::setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	if (red < 0) { red = 0; }
	else if (255 < red) { red = 255; }

	if (green < 0) { green = 0; }
	else if (255 < green) { green = 255; }

	if (blue < 0) { blue = 0; }
	else if (255 < blue) { blue = 255; }

	if (alpha < 0) { alpha = 0; }
	else if (255 < alpha) { alpha = 255; }

	SDL_SetRenderDrawColor(m_renderer, red, green, blue, alpha);
}

void Renderer::drawLine(Vector2D start, Vector2D end)
{
	// Make sure to convert the floats to int properly
	int startX = (int)(round(start.getX()));
	int startY = (int)(round(start.getY()));
	int endX = (int)(round(end.getX()));
	int endY = (int)(round(end.getY()));

	SDL_RenderDrawLine(m_renderer, startX, startY, endX, endY);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}

void Renderer::drawCircle(float centerX, float centerY, int radius, float rotation)
{
	s32 x = radius - 1;
	s32 y = 0;
	s32 tx = 1;
	s32 ty = 1;
	s32 err = tx - (radius << 1); // shifting bits left by 1 effectively
								  // doubles the value. == tx - diameter
	s32 workingCenterX = (int)round(centerX);
	s32 workingCenterY = (int)round(centerY);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(m_renderer, workingCenterX + x, workingCenterY - y);
		SDL_RenderDrawPoint(m_renderer, workingCenterX + x, workingCenterY + y);
		SDL_RenderDrawPoint(m_renderer, workingCenterX - x, workingCenterY - y);
		SDL_RenderDrawPoint(m_renderer, workingCenterX - x, workingCenterY + y);
		SDL_RenderDrawPoint(m_renderer, workingCenterX + y, workingCenterY - x);
		SDL_RenderDrawPoint(m_renderer, workingCenterX + y, workingCenterY + x);
		SDL_RenderDrawPoint(m_renderer, workingCenterX - y, workingCenterY - x);
		SDL_RenderDrawPoint(m_renderer, workingCenterX - y, workingCenterY + x);

		if (err <= 0)
		{
			y++;
			err += ty;
			ty += 2;
		}
		if (err > 0)
		{
			x--;
			tx += 2;
			err += tx - (radius << 1);
		}
	}

	float workingRadius = (float)radius;

	Vector2D center{ centerX, centerY };
	Vector2D top{ center.getX(), center.getY() - workingRadius };

	top = rotatePoint(top, center, rotation);

	drawLine(center, top);
}

void Renderer::drawCircle(Vector2D center, int radius, float rotation)
{
	s32 x = radius - 1;
	s32 y = 0;
	s32 tx = 1;
	s32 ty = 1;
	s32 err = tx - (radius << 1); // shifting bits left by 1 effectively
								  // doubles the value. == tx - diameter
	s32 centerX = (int)(round(center.getX()));
	s32 centerY = (int)(round(center.getY()));

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(m_renderer, centerX + x, centerY - y);
		SDL_RenderDrawPoint(m_renderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(m_renderer, centerX - x, centerY - y);
		SDL_RenderDrawPoint(m_renderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(m_renderer, centerX + y, centerY - x);
		SDL_RenderDrawPoint(m_renderer, centerX + y, centerY + x);
		SDL_RenderDrawPoint(m_renderer, centerX - y, centerY - x);
		SDL_RenderDrawPoint(m_renderer, centerX - y, centerY + x);

		if (err <= 0)
		{
			y++;
			err += ty;
			ty += 2;
		}
		if (err > 0)
		{
			x--;
			tx += 2;
			err += tx - (radius << 1);
		}
	}

	float workingRadius = (float)radius;

	Vector2D top{ center.getX(), center.getY() - workingRadius };

	top = rotatePoint(top, center, rotation);

	drawLine(center, top);
}

void Renderer::drawRect(
	float centerX, 
	float centerY, 
	int width, 
	int height, 
	bool filled)
{
	SDL_Rect rect;

	rect.x = (int)(round(centerX - (width / 2)));
	rect.y = (int)(round(centerY - (height / 2)));
	rect.w = width;
	rect.h = height;

	if(filled)
	{
		SDL_RenderFillRect(m_renderer, &rect);
	}
	else
	{
		SDL_RenderDrawRect(m_renderer, &rect);
	}
}

void Renderer::drawRect(Vector2D center, int width, int height, bool filled)
{
	SDL_Rect rect;

	int centerX = (int)(round(center.getX()));
	int centerY = (int)(round(center.getY()));

	rect.x = centerX - (width / 2);
	rect.y = centerY - (height / 2);
	rect.w = width;
	rect.h = height;

	if (filled)
	{
		SDL_RenderFillRect(m_renderer, &rect);
	}
	else
	{
		SDL_RenderDrawRect(m_renderer, &rect);
	}
}

void Renderer::drawRectEX(
	float centerX, 
	float centerY, 
	int width, 
	int height, 
	float rotation)
{
	Vector2D topLeft{ (centerX - (width / 2)), (centerY - (height / 2)) };
	Vector2D topRight{ (centerX + (width / 2)), (centerY - (height / 2)) };
	Vector2D bottomLeft{ (centerX - (width / 2)), (centerY + (height / 2)) };
	Vector2D bottomRight{ (centerX + (width / 2)), (centerY + (height / 2)) };
	Vector2D center{ centerX, centerY };
	Vector2D topCenter{ centerX, centerY - (height / 2) };

	// Rotate the rectangles points
	topLeft = rotatePoint(topLeft, center, rotation);
	topRight = rotatePoint(topRight, center, rotation);
	bottomLeft = rotatePoint(bottomLeft, center, rotation);
	bottomRight = rotatePoint(bottomRight, center, rotation);
	topCenter = rotatePoint(topCenter, center, rotation);

	// Draw the rectangle
	drawLine(topLeft, topRight);
	drawLine(topRight, bottomRight);
	drawLine(bottomRight, bottomLeft);
	drawLine(bottomLeft, topLeft);

	// Draw the line showing the top of the rectangle's location
	drawLine(center, topCenter);
}

void Renderer::drawRectEX(Vector2D center, int width, int height, float rotation)
{
	float halfWidth = (float)(width) / 2.0f;
	float halfHeight = (float)(height) / 2.0f;

	// Get the corner coordinates for the rectangle
	Vector2D topLeft{ center.getX() - halfWidth, center.getY() - halfHeight };
	Vector2D topRight{ center.getX() + halfWidth, center.getY() - halfHeight };
	Vector2D bottomLeft{ center.getX() - halfWidth, center.getY() + halfHeight };
	Vector2D bottomRight{ center.getX() + halfWidth, center.getY() + halfHeight };
	Vector2D topCenter{ center.getX(), center.getY() - halfHeight };

	// Rotate the rectangles points
	topLeft = rotatePoint(topLeft, center, rotation);
	topRight = rotatePoint(topRight, center, rotation);
	bottomLeft = rotatePoint(bottomLeft, center, rotation);
	bottomRight = rotatePoint(bottomRight, center, rotation);
	topCenter = rotatePoint(topCenter, center, rotation);

	// Draw the rectangle
	drawLine(topLeft, topRight);
	drawLine(topRight, bottomRight);
	drawLine(bottomRight, bottomLeft);
	drawLine(bottomLeft, topLeft);

	// Draw the line showing the top of the rectangle's location
	drawLine(center, topCenter);
}

//=============================================================================
// Function: void renderTexture(Texture*, int, int, SDL_Rect*)
// Description:
// Draws the texture to the renderer. 
// Parameters:
// Texture *texture - The texture to render.
// int x - The x position to render it at.
// int y - The y position to render it at.
// SDL_Rect *clip - The texture clip to use. NULL if using the entire texture.
//=============================================================================
void Renderer::renderTexture(Texture *texture, int x, int y, SDL_Rect *clip)
{
	if(texture)
	{
		int width, height;
		
		if (clip)
		{
			width = clip->w;
			height = clip->h;
		}
		else
		{
			width = (int)round(texture->center().getX() * 2);
			height = (int)round(texture->center().getY() * 2);
		}

		SDL_Rect renderRect{ x, y, width, height };

		SDL_RenderCopy(m_renderer, texture->texture(), clip, &renderRect);
	}
}

//============================================================================
// Function: void renderTextureEX(Texture*, int, int, SDL_Rect*, 
//                                float, SDL_RendererFlip)
// Description:
// 
// Parameters:
// Texture *texture - The texture to render.
// int x - The x position to render it at.
// int y - The y position to render it at.
// Vector2D *anchor - The point to rotate the texture around. NULL if center.
// SDL_Rect *destination - The destination rect. NULL if the entire target.
// SDL_Rect *clip - The clip to use, NULL if using the entire target.
// float rotation - The amount to rotate the texture, 0.0f if no rotation.
// SDL_RendererFlip flip - The ways to flip it:
//		-SDL_FLIP_NONE = No flip.
//      -SDL_FLIP_HORIZONTAL = Horizontal flip.
//		-SDL_FLIP_VERTICAL = Vertical flip.
//=============================================================================
void Renderer::renderTextureEX(
	Texture *texture, 
	int x, 
	int y,
	Vector2D *anchor,
	SDL_Rect *destination,
	SDL_Rect *clip,
	float rotation,
	SDL_RendererFlip flip)
{
	if (texture)
	{
		SDL_Point anchorPoint;

		if(anchor)
		{
			anchorPoint.x = (int)round(anchor->getX());
			anchorPoint.y = (int)round(anchor->getY());
		}
		else
		{
			if(clip)
			{
				anchorPoint.x = (clip->w / 2);
				anchorPoint.y = (clip->h / 2);
			}
			else
			{
				anchorPoint.x = (int)round(texture->center().getX());
				anchorPoint.y = (int)round(texture->center().getY());
			}
		}

		SDL_RenderCopyEx(
			m_renderer,
			texture->texture(),
			clip,
			destination,
			(double)rotation,
			&anchorPoint,
			flip);
	}
}