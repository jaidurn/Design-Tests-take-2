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
#include "UIText.h"
#include "UIGrid.h"
#include "UIButton.h"
#include "UIGraphic.h"

Game::Game()
	:m_physicsSys(NULL),
	m_renderSys(NULL),
	m_messageSys(NULL),
	m_inputSys(NULL), 
	m_logicSys(NULL),
	m_initialized(false), 
	m_currentState(GS_EXIT),
	m_resource(NULL),
	m_text(NULL),
	m_list(NULL)
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

	delete m_world;
	m_world = NULL;

	delete m_text;
	m_text = NULL;

	delete m_list;
	m_list = NULL;

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
					while (!m_world->exists())
					{
						m_world->dungeon();
					}

					int player = EntitySystem::instance()->createEntity(0, m_world->getRoom(0)->rect()->center());
					int enemy = EntitySystem::instance()->createEntity(1, Vector2D(200, 100));

					Camera2D *camera = RenderSystem::instance()->camera();

					Vector2D cameraCenter(camera->getX() + (camera->getWidth() / 2),
						camera->getY() + (camera->getHeight() / 2));

					m_renderSys->setCameraTarget(player);
					m_renderSys->camera()->setBoundingBoxSize(ResourceManager::instance()->window()->width() / 4, ResourceManager::instance()->window()->height() / 4);
					m_renderSys->setSpriteLayer(player, RenderSystem::RENDER_FOREGROUND1);

					m_inputSys->createInputComponent(player)->addDevice(m_inputSys->getNextFreeDevice());

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

					Font *font = m_resource->getFont("Resources/Fonts/RobotoMono-", 16, Font::FONT_REGULAR, false);

					int textID = EntitySystem::instance()->createEntity();
					Vector2D textPos(camera->getX() + 200, camera->getY() + 100);
					SDL_Color red{ 255, 0, 0, 255 };
					m_text = new UIText(textID, textPos, "Text");
					m_renderSys->createTextComponent(textID, "Test", font, red, 0, textPos);

					int listID = EntitySystem::instance()->createEntity();
					Vector2D listPos(camera->getX() + camera->getWidth() - 50, camera->getY() + (camera->getHeight() / 2));

					m_list = new UIGrid(listID, "Group0", listPos, 100, camera->getHeight(), 2, 2);

					int buttonID = EntitySystem::instance()->createEntity();
					Vector2D buttonPos(0, 0);

					UIButton *button0 = new UIButton(buttonID, buttonPos, "0", false);
					
					textID = EntitySystem::instance()->createEntity();

					UIText *text0 = new UIText(textID, buttonPos, "t0");

					m_renderSys->createTextComponent(textID, "Item 0", font, red, 0, textPos);

					int graphicID = EntitySystem::instance()->createEntity();

					UIGraphic *graphic0 = new UIGraphic(graphicID, buttonPos, "g0");

					m_renderSys->createSprite(graphicID, "Resources/cardBase.png", buttonPos);
					m_renderSys->setSpriteLayer(graphicID, RenderSystem::RENDER_UI_BACKGROUND);

					button0->setBackground(graphic0);
					button0->setTextUI(text0);

					// Button 1
					buttonID = EntitySystem::instance()->createEntity();

					UIButton *button1 = new UIButton(buttonID, buttonPos, "1", false);

					textID = EntitySystem::instance()->createEntity();

					UIText *text1 = new UIText(textID, buttonPos, "t1");

					m_renderSys->createTextComponent(textID, "Item 1", font, red, 0, textPos);

					graphicID = EntitySystem::instance()->createEntity();

					UIGraphic *graphic1 = new UIGraphic(graphicID, buttonPos, "g1");

					m_renderSys->createSprite(graphicID, "Resources/cardBase.png", buttonPos);
					m_renderSys->setSpriteLayer(graphicID, RenderSystem::RENDER_UI_BACKGROUND);

					button1->setBackground(graphic1);
					button1->setTextUI(text1);

					// Button 2
					buttonID = EntitySystem::instance()->createEntity();

					UIButton *button2 = new UIButton(buttonID, buttonPos, "2", false);

					textID = EntitySystem::instance()->createEntity();

					UIText *text2 = new UIText(textID, buttonPos, "t2");

					m_renderSys->createTextComponent(textID, "Item 2", font, red, 0, textPos);

					graphicID = EntitySystem::instance()->createEntity();

					UIGraphic *graphic2 = new UIGraphic(graphicID, buttonPos, "g2");

					m_renderSys->createSprite(graphicID, "Resources/cardBase.png", buttonPos);
					m_renderSys->setSpriteLayer(graphicID, RenderSystem::RENDER_UI_BACKGROUND);

					button2->setBackground(graphic2);
					button2->setTextUI(text2);

					// Button 3
					buttonID = EntitySystem::instance()->createEntity();

					UIButton *button3 = new UIButton(buttonID, buttonPos, "3", false);

					textID = EntitySystem::instance()->createEntity();

					UIText *text3 = new UIText(textID, buttonPos, "t3");

					m_renderSys->createTextComponent(textID, "Item 3", font, red, 0, textPos);

					graphicID = EntitySystem::instance()->createEntity();

					UIGraphic *graphic3 = new UIGraphic(graphicID, buttonPos, "g3");

					m_renderSys->createSprite(graphicID, "Resources/cardBase.png", buttonPos);
					m_renderSys->setSpriteLayer(graphicID, RenderSystem::RENDER_UI_BACKGROUND);

					button3->setBackground(graphic3);
					button3->setTextUI(text3);

					// Button 4
					buttonID = EntitySystem::instance()->createEntity();

					UIButton *button4 = new UIButton(buttonID, buttonPos, "4", false);

					textID = EntitySystem::instance()->createEntity();

					UIText *text4 = new UIText(textID, buttonPos, "t4");

					m_renderSys->createTextComponent(textID, "Item 4", font, red, 0, buttonPos);

					graphicID = EntitySystem::instance()->createEntity();

					UIGraphic *graphic4 = new UIGraphic(graphicID, buttonPos, "g4");

					m_renderSys->createSprite(graphicID, "Resources/cardBase.png", buttonPos);
					m_renderSys->setSpriteLayer(graphicID, RenderSystem::RENDER_UI_BACKGROUND);

					button4->setBackground(graphic4);
					button4->setTextUI(text4);

					m_list->addItem(button0);
					m_list->addItem(button1);
					m_list->addItem(button2);
					m_list->addItem(button3);
					m_list->addItem(button4);

					m_text->setWidth(200);
					m_text->setHeight(100);

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
		
		m_list->update();

		processPhysics(deltaTime);

		m_world->renderRooms();

		prevTime = m_timer.currentSeconds() - startTime;
		startTime = m_timer.currentSeconds();

		updateRenderer(deltaTime);

		if(counter % 10 == 0)
		{
			static float renderTime = 0;

			renderTime = m_timer.currentSeconds() - startTime;
			//std::cout << "Render time: " << renderTime << ", Prev time: " << prevTime << std::endl;
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
				else if (e.key.keysym.sym == SDLK_0)
				{
					m_text->setText("Short text!");
				}
				else if (e.key.keysym.sym == SDLK_1)
				{
					m_text->setText("Medium text ahahaha!");
				}
				else if (e.key.keysym.sym == SDLK_2)
				{
					m_text->setText("Looooooooooo ng text is here boyos it's here!");
				}
				else if (e.key.keysym.sym == SDLK_3)
				{
					m_text->setText("Test");
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
				m_list->processMessage(message);
				EntitySystem::instance()->processMessage(message);
				m_logicSys->processMessage(message);
				m_physicsSys->processMessage(message);
				m_renderSys->processMessage(message);
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