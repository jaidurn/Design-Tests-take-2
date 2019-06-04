#pragma once
//==========================================================================================
// File Name: Window.h
// Author: Brian Blackmon
// Date Created: 3/19/2019
// Purpose: 
// Wraps the SDL_Window struct and adds functionality.
//==========================================================================================
#include <string>
#include <SDL.h>

struct SDL_Window;

class Window
{
public:
	Window(const char *title, int width, int height);
	Window(std::string title, int width, int height, Uint32 flags);
	~Window();

	// Accessors
	const char* title();
	int width();
	int height();
	Uint32 pixelFormat();

	// Mutators
	void resize(int width, int height);
	void setTitle(const char *title);
	void setFullscreen();
	void setWindowed();

	SDL_Window* window() { return m_window; }

private:
	SDL_Window *m_window;
};

