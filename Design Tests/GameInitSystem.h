#pragma once
//==========================================================================================
// File Name: GameInitSystem.h
// Author: Brian Blackmon
// Date Created: 5/1/2019
// Purpose: 
// Initializes the game information and game systems. This will set up the physics system,
// render system, and everything else.
//==========================================================================================
#include <string>
#include <SDL.h>
#include "ResourceManager.h"
#include "SettingIO.h"

class GameInitSystem
{
public:
	~GameInitSystem();

	static GameInitSystem* instance()
	{
		static GameInitSystem *instance = new GameInitSystem();

		return instance;
	}

	void initialize(std::string settingsFile);

private:
	GameInitSystem()
		:m_initialized(false)
	{
	}

	SettingIO m_settingsManager;
	bool m_initialized;

	void loadWindow();
	void loadVideo();
	void loadPhysics();
	void loadInput();
	void loadEntity();
};

