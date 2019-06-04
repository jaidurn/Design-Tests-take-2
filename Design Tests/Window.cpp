#include "Window.h"
#include "ErrorSystem.h"

Window::Window(const char *title, int width, int height)
	:m_window(NULL)
{
	m_window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN);

	if(!m_window)
	{
		// Destroy itself
		ErrorSystem::logError("Failed to create window!");
		this->~Window();
	}
}

Window::Window(std::string title, int width, int height, Uint32 flags)
	:m_window(NULL)
{
	m_window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		flags);

	if(!m_window)
	{
		ErrorSystem::logError("Failed to create window!");
		this->~Window();
	}
}

Window::~Window()
{
	if (m_window) { SDL_DestroyWindow(m_window); }
}

const char* Window::title()
{
	return SDL_GetWindowTitle(m_window);
}

int Window::width()
{
	int width{ 0 };

	SDL_GetWindowSize(m_window, &width, NULL);

	return width;
}

int Window::height()
{
	int height{ 0 };

	SDL_GetWindowSize(m_window, NULL, &height);

	return height;
}

Uint32 Window::pixelFormat()
{
	return SDL_GetWindowPixelFormat(m_window);
}

void Window::resize(int width, int height)
{
	int workingWidth{ 0 };
	int workingHeight{ 0 };

	if (0 < width) { workingWidth = width; }
	if (0 < height) { workingHeight = height; }

	SDL_SetWindowSize(m_window, workingWidth, workingHeight);
}

void Window::setTitle(const char *title)
{
	if (title) { SDL_SetWindowTitle(m_window, title); }
}

void Window::setFullscreen()
{
	SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
}

void Window::setWindowed()
{
	SDL_SetWindowFullscreen(m_window, 0);
}