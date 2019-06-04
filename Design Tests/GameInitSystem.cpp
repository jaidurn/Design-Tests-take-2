#include "GameInitSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "EntitySystem.h"

GameInitSystem::~GameInitSystem()
{
	m_initialized = false;
}

//=============================================================================
// Function: void initialize(string)
// Description:
// Initializes all of the systems required for the game.
// Parameters:
// string settingsFile - The file to load all the settings from.
//=============================================================================
void GameInitSystem::initialize(std::string settingsFile)
{
	if (!m_initialized)
	{
		m_settingsManager.open(settingsFile, SettingIO::READ);

		if (m_settingsManager.isOpen())
		{
			loadWindow();
			loadVideo();
			loadPhysics();
			loadInput();
			loadEntity();
		}
		
		m_initialized = true;
	}
}

//=============================================================================
// Function: void loadWindow()
// Description:
// Loads the window information from the settings file and initializes
// the window.
//=============================================================================
void GameInitSystem::loadWindow()
{
	std::string title = m_settingsManager.loadSetting("WindowTitle");
	std::string width = m_settingsManager.loadSetting("WindowWidth");
	std::string height = m_settingsManager.loadSetting("WindowHeight");

	// Load every window flag here
	Uint32 flags = SDL_WINDOW_SHOWN;

	if (m_settingsManager.loadSetting("WindowFullscreen") == "1")
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	if(m_settingsManager.loadSetting("WindowBorderless") == "1")
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}

	if (title == "") { title = ""; }
	if (width == "") { width = "1280"; }
	if (height == "") { height = "720"; }

	ResourceManager::instance()->initWindow(title, std::stoi(width), std::stoi(height), flags);
}

//=============================================================================
// Function: void loadVideo()
// Description:
// Loads the video components for the game.
//=============================================================================
void GameInitSystem::loadVideo()
{
	bool vSync = false;

	if(m_settingsManager.loadSetting("VSyncEnabled") == "1")
	{
		vSync = true;
	}

	ResourceManager::instance()->initRenderer(vSync);
	RenderSystem::instance()->setRenderer(ResourceManager::instance()->renderer());

	std::string s_baseWidth = m_settingsManager.loadSetting("BaseWidth");
	std::string s_baseHeight = m_settingsManager.loadSetting("BaseHeight");
	int baseWidth = std::stoi(s_baseWidth);
	int baseHeight = std::stoi(s_baseHeight);

	SDL_Rect rect{ 0, 0, baseWidth, baseHeight };

	float scaleX = (float)ResourceManager::instance()->window()->width() / (float)baseWidth;
	float scaleY = (float)ResourceManager::instance()->window()->height() / (float)baseHeight;

	RenderSystem::instance()->createCamera(rect, scaleX, scaleY);
}

//=============================================================================
// Function: void loadPhysics()
// Description:
// Loads the physics system and sets up the grid.
//=============================================================================
void GameInitSystem::loadPhysics()
{
	int originX = std::stoi(m_settingsManager.loadSetting("GridOriginX"));
	int originY = std::stoi(m_settingsManager.loadSetting("GridOriginY"));

	int width = 1280;
	int height = 720;

	int cellSize = std::stoi(m_settingsManager.loadSetting("GridCellSize"));

	PhysicsSystem::instance()->initCollisionSystem(originX, originY, width, height, cellSize);
}

//=============================================================================
// Function: void loadInput()
// Description:
// Loads up the input system.
//=============================================================================
void GameInitSystem::loadInput()
{
	Input::InputSystem::instance();
}

//=============================================================================
// Function: void loadEntity()
// Description:
// Loads up the entity system.
//=============================================================================
void GameInitSystem::loadEntity()
{
	EntitySystem *entity = EntitySystem::instance();
	std::string entityDataPath = m_settingsManager.loadSetting("EntityDataFile");
	if (entityDataPath != "")
	{
		entity->initialize(entityDataPath);
	}
}