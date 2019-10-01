#pragma once
//==========================================================================================
// File Name: Game.h
// Author: Brian Blackmon
// Date Created: 7/5/2019
// Purpose: 
// The core class for the game. Handles start up and running of the game.
//==========================================================================================
#include "Timer.h"
#include <string>

class TextComponent;
class ResourceManager;
class PhysicsSystem;
class RenderSystem;
class MessageSystem;
class LogicSystem;
class InputSystem;
class World;
class Room;
class UIText;
class UIGrid;

class Game
{
public:
	enum GameState
	{
		GS_EXIT,
		GS_RUNNING,
		GS_PAUSED
	};

	static Game* instance()
	{
		static Game *instance = new Game();

		return instance;
	}

	~Game();

	bool init(std::string gamePath);
	
	void loop();

	GameState state() { return m_currentState; }

private:
	Game();

	ResourceManager *m_resource;
	PhysicsSystem *m_physicsSys;
	RenderSystem *m_renderSys;
	MessageSystem *m_messageSys;
	InputSystem *m_inputSys;
	LogicSystem *m_logicSys;
	World *m_world;

	UIText *m_text;
	UIGrid *m_list;

	Timer m_timer;

	bool m_initialized;
	GameState m_currentState;

	void processInput();
	void processMessages();
	void processLogic(float delta);
	void processPhysics(float delta);
	void clearRenderer();
	void updateRenderer(float delta);
};

