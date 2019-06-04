#pragma once
//==========================================================================================
// File Name: SpriteParser.h
// Author: Brian Blackmon
// Date Created: 5/16/2019
// Purpose: 
// Parses sprite information and creates animation components.
//==========================================================================================
#include "RenderSystem.h"
#include <SDL.h>

class Texture;

class SpriteParser
{
public:
	SpriteParser();
	~SpriteParser();

	void loadSprite(Texture *texture, int columnNum, int rowNum);

private:
};

