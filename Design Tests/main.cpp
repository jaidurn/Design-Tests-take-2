#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Game.h"

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		std::cout << "Failed to initialize SDL with error: "
			<< SDL_GetError()
			<< std::endl;

		return 1;
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		std::cout << "Failed to initialize SDL_Image with error: "
			<< IMG_GetError()
			<< std::endl;

		return 1;
	}

	if(TTF_Init() == -1)
	{
		std::cout << "Failed to initialize SDL_TTF with error: "
			<< TTF_GetError()
			<< std::endl;

		return 1;
	}

	Game *game = Game::instance();

	if (!game->init("Resources/load.ini"))
	{
		std::cout << "Failed to load the game!\n";
	}
	else
	{
		while (game->state() != game->GS_EXIT)
		{
			game->loop();
		}
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}