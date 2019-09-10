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
#include "UIText.h"
#include "UIMenu.h"
#include "UIGraphic.h"
#include "UIDeckGrid.h"
#include "Deck.h"

Game::Game()
	:m_physicsSys(NULL),
	m_renderSys(NULL),
	m_messageSys(NULL),
	m_inputSys(NULL), 
	m_logicSys(NULL),
	m_initialized(false), 
	m_currentState(GS_EXIT),
	m_resource(NULL), 
	m_menu(NULL)
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

	delete m_menu;
	m_menu = NULL;

	delete m_world;
	m_world = NULL;

	m_timer.stop();
}

void press()
{
	std::cout << "Pressed!\n";
}

void release()
{
	std::cout << "Released!\n";
}

void hold()
{
	static int time = 0;

	if(time % 10 == 0)
	{
		std::cout << "YEE HAW!\n";
	}

	time++;
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
					while (!m_world->exists())
					{
						m_world->dungeon();
					}

					int player = EntitySystem::instance()->createEntity(0, m_world->getRoom(0)->rect()->center());
					int enemy = EntitySystem::instance()->createEntity(1, Vector2D(200, 100));

					Camera2D *camera = RenderSystem::instance()->camera();

					Vector2D cameraCenter(camera->getX() + (camera->getWidth() / 2),
						camera->getY() + (camera->getHeight() / 2));

					Deck *deck = new Deck();

					int cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_COMMON);

					Rectangle *cardRect = new Rectangle(0, 0, 64, 128);

					UIButton *card0 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardWorld.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_UNCOMMON);

					UIButton *card1 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardWorld.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_RARE);

					UIButton *card2 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardWorld.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_STAT, Card::CARD_COMMON);

					UIButton *card3 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardBase.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_STAT, Card::CARD_COMMON);

					UIButton *card4 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardBase.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_COMMON);

					UIButton *card5 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardWorld.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_COMMON);

					UIButton *card6 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardWorld.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_COMMON);

					UIButton *card7 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardBase.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_COMMON);

					UIButton *card8 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardBase.png");

					cardID = EntitySystem::instance()->createEntity();
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_COMMON);

					UIButton *card9 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardBase.png");

					cardID = EntitySystem::instance()->createEntity();
					
					deck->addCard(cardID, Card::CARD_WORLD, Card::CARD_COMMON);

					UIButton *card10 = new UIButton(cardID, Vector2D(0, 0), cardRect, "Resources/cardBase.png");


					m_menu = new UIDeckGrid(cameraCenter, (camera->getWidth() - 160),
						(camera->getHeight() - 40), "Resources/menu.png", 
						8, 4, deck);

					m_menu->addItem(card0->getEntityID(), card0);
					m_menu->addItem(card1->getEntityID(), card1);
					m_menu->addItem(card2->getEntityID(), card2);
					m_menu->addItem(card3->getEntityID(), card3);
					m_menu->addItem(card4->getEntityID(), card4);
					m_menu->addItem(card5->getEntityID(), card5);
					m_menu->addItem(card6->getEntityID(), card6);
					m_menu->addItem(card7->getEntityID(), card7);
					m_menu->addItem(card8->getEntityID(), card8);
					m_menu->addItem(card9->getEntityID(), card9);
					m_menu->addItem(card10->getEntityID(), card10);

					m_menu->setVisible(false);
					m_menu->setActive(false);

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

		m_menu->update(deltaTime);

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
				else if(e.key.keysym.sym == SDLK_ESCAPE)
				{
					if(m_menu->getVisible())
					{
						m_menu->setVisible(false);
						m_menu->setActive(false);
					}
					else
					{
						m_menu->setVisible(true);
						m_menu->setActive(true);
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
				m_menu->processMessage(message);
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
		m_renderSys->update(delta);
	}
}