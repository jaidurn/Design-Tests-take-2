#include "Game.h"
#include "MessageSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "LogicSystem.h"
#include "InputSystem.h"
#include "GameInitSystem.h"
#include "EntitySystem.h"
#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "EnemyAttackState.h"
#include "EnemyTargetState.h"
#include "EnemyIdleState.h"
#include "World.h"
#include "UIButton.h"

Game::Game()
	:m_physicsSys(NULL), m_renderSys(NULL), m_messageSys(NULL),
	m_inputSys(NULL), m_logicSys(NULL), m_initialized(false), m_currentState(GS_EXIT),
	m_button(NULL), m_room(NULL), m_resource(NULL)
{
	m_world = new World("Resources/rooms.dat");
}


Game::~Game()
{
	m_resource = NULL;
	m_physicsSys = NULL;
	m_renderSys = NULL;
	m_messageSys = NULL;
	m_inputSys = NULL;
	m_logicSys = NULL;

	delete m_button;
	m_button = NULL;

	delete m_world;
	m_world = NULL;

	m_timer.stop();
}

//=============================================================================
// Function: bool init(string)
// Description:
// Initializes all of the game's systems specified by the game file.
// Parameters:
// string gamePath - The path to the game data file.
// Output:
// Returns true if initialization was successful.
// Returns false on failure.
//=============================================================================
bool Game::init(std::string gamePath)
{
	bool success = false;

	if (!m_initialized)
	{
		if (gamePath != "")
		{
			if (GameInitSystem::instance()->initialize(gamePath))
			{
				m_resource = ResourceManager::instance();
				m_physicsSys = PhysicsSystem::instance();
				m_renderSys = RenderSystem::instance();
				m_messageSys = MessageSystem::instance();
				m_logicSys = LogicSystem::instance();
				m_inputSys = InputSystem::instance();

				if (m_physicsSys && m_renderSys && m_messageSys && m_logicSys && m_inputSys)
				{
					while(!m_world->exists())
					{
						m_world->dungeon();
					}

					int player = EntitySystem::instance()->createEntity(0, m_world->getRoom(0)->rect()->center());
					int enemy = EntitySystem::instance()->createEntity(1, Vector2D(200, 100));

					m_renderSys->setCameraTarget(player);
					m_renderSys->camera()->setBoundingBoxSize(ResourceManager::instance()->window()->width() / 4, ResourceManager::instance()->window()->height() / 4);
					m_renderSys->setSpriteLayer(player, RenderSystem::RENDER_FOREGROUND1);

					m_inputSys->createInputComponent(player)->addDevice(m_inputSys->getNextFreeDevice());
					m_inputSys->getInputComponent(player)->addDevice(m_inputSys->getNextFreeDevice());
					m_inputSys->getInputComponent(player)->addDevice(m_inputSys->getNextFreeDevice());

					CollisionComponent *playerCol = m_physicsSys->getCollisionComponent(player);
					CollisionComponent *enemyCol = m_physicsSys->getCollisionComponent(enemy);

					LogicComponent *playerLog = m_logicSys->createLogicComponent(player, LogicComponent::LOGIC_PLAYER);
					LogicComponent *enemyLog = m_logicSys->createLogicComponent(enemy, LogicComponent::LOGIC_ENEMY);

					PlayerIdleState *idle1 = new PlayerIdleState(player, "Idle");
					PlayerMoveState *move1 = new PlayerMoveState(player, "Walk", 60.0f);
					EnemyAttackState *move4 = new EnemyAttackState(enemy, 2.0f);
					EnemyTargetState *target4 = new EnemyTargetState(enemy, 2.0f, false, "Player", 100);
					EnemyIdleState *idle4 = new EnemyIdleState(enemy, 1.0f);

					playerLog->addState("Idle", idle1);
					playerLog->addState("Walk", move1);
					enemyLog->addState("Target", target4);
					enemyLog->addState("Attack", move4);
					enemyLog->addState("Idle", idle4);

					playerLog->changeState("Idle");

					Camera2D *camera = RenderSystem::instance()->camera();

					m_button = new UIButton(-1, Vector2D((float)(80), (float)(80)), NULL, "Resources/button.png");

					Shape::Rectangle *rect = new Shape::Rectangle(m_button->getPosition().getX(), m_button->getPosition().getY(), 100, 100);

					m_button->setRect(rect);

					m_initialized = true;
					success = true;
					m_timer.start();
					m_currentState = GS_RUNNING;
				}
			}
		}
	}

	return success;
}

//=============================================================================
// Function: void loop()
// Description:
// Handles the game update loop. 
//=============================================================================
void Game::loop()
{
	if (m_currentState == GS_RUNNING)
	{
		static float startTime = m_timer.currentSeconds();
		static float prevTime = 0;
		static int counter = 0;

		clearRenderer();

		processMessages();

		float deltaTime = m_timer.currentSeconds() - startTime;

		processInput();
		processLogic(deltaTime);
		processPhysics(deltaTime);

		if(m_button->getPressed())
		{
			std::cout << "Pressed\n";
		}
		else if (m_button->getReleased())
		{
			std::cout << "Released!\n";
		}

		m_button->update(deltaTime);

		m_world->renderRooms();

		prevTime = m_timer.currentSeconds() - startTime;
		startTime = m_timer.currentSeconds();

		updateRenderer(deltaTime);

		if(counter % 10 == 0)
		{
			static float renderTime = 0;

			renderTime = m_timer.currentSeconds() - startTime;
			std::cout << "Render time: " << renderTime << ", Prev time: " << prevTime << std::endl;
		}

		counter++;
	}
} 

//=============================================================================
// Function: void processInput()
// Description:
// Polls the system for user input and handles it.
//=============================================================================
void Game::processInput()
{
	if (m_initialized)
	{
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			m_inputSys->processInput(e);

			if(e.type == SDL_QUIT)
			{
				m_currentState = GS_EXIT;
			}

			if(e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_BACKSLASH)
				{
					if(m_world->exists())
					{
						m_world->clear();
					}
					else
					{
						while (!m_world->exists())
						{
							m_world->dungeon();
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// Function: void processMessages()
// Description:
// Processes the system messages.
//=============================================================================
void Game::processMessages()
{
	if (m_initialized)
	{
		IMessage *message = NULL;

		while (m_messageSys->pollMessage(message))
		{
			if (message)
			{
				EntitySystem::instance()->processMessage(message);
				m_logicSys->processMessage(message);
				m_physicsSys->processMessage(message);
				m_renderSys->processMessage(message);
				m_button->processMessage(message);
			}
		}
	}
}

//=============================================================================
// Function: void processLogic(float)
// Description:
// Processes the logic of the game.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void Game::processLogic(float delta)
{
	if (m_initialized)
	{
		m_logicSys->update(delta);
	}
}

//=============================================================================
// Function: void processPhysics(float)
// Description:
// Processes the physics.
// Parameters:
// float delta - The time passed since the last update.
//=============================================================================
void Game::processPhysics(float delta)
{
	if (m_initialized)
	{
		m_physicsSys->update(delta);
	}
}

//=============================================================================
// Function: void clearRenderer()
// Description:
// Clears the renderer to prepare it for the next frame.
//=============================================================================
void Game::clearRenderer()
{
	if (m_initialized)
	{
		m_renderSys->clear();
	}
}

//=============================================================================
// Function: void updateRenderer(float)
// Description:
// Updates the renderer and draws all of the active game objects.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void Game::updateRenderer(float delta)
{
	if(m_initialized)
	{
		Line left{ m_button->getRect()->getTopLeft(), m_button->getRect()->getBottomLeft() };
		Line right{ m_button->getRect()->getTopRight(), m_button->getRect()->getBottomRight() };
		Line top{ m_button->getRect()->getTopLeft(), m_button->getRect()->getTopRight() };
		Line bottom{ left.end, right.end };
		SDL_Color red{ 255, 0, 0, 255 };

		m_renderSys->drawLine(left, red);
		m_renderSys->drawLine(right, red);
		m_renderSys->drawLine(top, red);
		m_renderSys->drawLine(bottom, red);
		m_renderSys->update(delta);
	}
}