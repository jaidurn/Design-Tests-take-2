#pragma once
//==========================================================================================
// File Name: Renderer.h
// Author: Brian Blackmon
// Date Created: 3/19/2019
// Purpose: 
// Wraps the SDL_Renderer struct and adds extra functionality.
//==========================================================================================
#include <SDL.h>

struct SDL_Renderer;
struct SDL_Window;
class Vector2D;
class Texture;

typedef int32_t s32;

class Renderer
{
public:
	Renderer(SDL_Window *window);
	Renderer(SDL_Window *window, bool vSyncEnabled);
	~Renderer();

	SDL_Renderer* renderer() { return m_renderer; }
	bool hasPixelFormat(Uint32 format);
	Uint32 getAlphaPixelFormat();

	// Renderer tools
	void renderPresent();
	void clear();

	// Drawing tools
	void setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	void drawLine(Vector2D start, Vector2D end);
	void drawLine(int x1, int y1, int x2, int y2);
	
	void drawCircle(float centerX, float centerY, int radius, float rotation = 0.0f);
	void drawCircle(Vector2D center, int radius, float rotation = 0.0f);

	void drawRect(float centerX, float centerY, int width, int height, bool filled = false);
	void drawRect(Vector2D center, int width, int height, bool filled = false);

	void drawRectEX(float centerX, float centerY, int width, int height, float rotation);
	void drawRectEX(Vector2D center, int width, int height, float rotation);

	// Texture tools
	void renderTexture(Texture *texture, int x, int y, SDL_Rect *clip = NULL);
	void renderTextureEX(Texture *texture, int x, int y, Vector2D *anchor, SDL_Rect *destination, SDL_Rect *clip = NULL, float rotation = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	SDL_Renderer *m_renderer;
};

