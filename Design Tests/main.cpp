#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

#include "ResourceManager.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "Controller.h"
#include "InputComponent.h"
#include "GameInitSystem.h"
#include "EntitySystem.h"
#include "SettingIO.h"
#include "IMessage.h"
#include "MoveMessage.h"
#include "StateChangeMessage.h"
#include "CollisionMessage.h"
#include "LogicSystem.h"
#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "EnemyIdleState.h"
#include "EnemyAttackState.h"
#include "EnemyTargetState.h"

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

	GameInitSystem *init = GameInitSystem::instance();

	init->initialize("Resources/load.ini");

	ResourceManager *resource = ResourceManager::instance();
	RenderSystem *render = RenderSystem::instance();
	PhysicsSystem *phys = PhysicsSystem::instance();
	Input::InputSystem *input = Input::InputSystem::instance();
	LogicSystem *logic = LogicSystem::instance();

	int player = EntitySystem::instance()->createEntity("0", Vector2D(50, 50));
	int enemy = EntitySystem::instance()->createEntity("1", Vector2D(200, 100));
	int player2 = EntitySystem::instance()->createEntity("0", Vector2D(700, 50));
	int enemy2 = EntitySystem::instance()->createEntity("1", Vector2D(400, 100));

	input->createInputComponent(2);
	input->createInputComponent(0)->addDevice(input->getNextFreeDevice());
	input->getInputComponent(0)->addDevice(input->getNextFreeDevice());
	input->getInputComponent(0)->addDevice(input->getNextFreeDevice());
	input->createInputComponent(3)->addDevice(input->getNextFreeDevice());

	LogicComponent *playerLog = logic->createLogicComponent(player, LogicComponent::LOGIC_PLAYER);
	LogicComponent *player2Log = logic->createLogicComponent(player2, LogicComponent::LOGIC_PLAYER);
	LogicComponent *enemyLog = logic->createLogicComponent(enemy, LogicComponent::LOGIC_ENEMY);
	LogicComponent *enemy2Log = logic->createLogicComponent(enemy2, LogicComponent::LOGIC_ENEMY);

	PlayerIdleState *idle1 = new PlayerIdleState(player, "Idle");
	PlayerMoveState *move1 = new PlayerMoveState(player, "Walk", 2.0f);
	EnemyAttackState *move4 = new EnemyAttackState(enemy, 2.0f);
	EnemyTargetState *target4 = new EnemyTargetState(enemy, 2.0f, false, "Player", 100);

	Line lineOfSight{ Vector2D(50, 50), Vector2D(200, 100) };

	SDL_Color red{ 255, 0, 0, 255 };
	SDL_Color green{ 0, 255, 0, 255 };

	playerLog->addState("Idle", idle1);
	playerLog->addState("Walk", move1);
	enemyLog->addState("Target", target4);
	enemyLog->addState("Attack", move4);

	playerLog->changeState("Idle");
	player2Log->changeState("Idle");

	IMessage *message = NULL;

	SDL_Event e;

	while (!input->exit())
	{
		static int counter = 0;

		Uint32 ticks = SDL_GetTicks();

		while (SDL_PollEvent(&e))
		{
			input->processInput(e);
			Input::InputDevice *device = input->getDevice(1);

			static bool pressed = false;
			static bool proccessed = false;

			if(device->buttonPressed(SDLK_UP))
			{
				pressed = true;
			}
			else if(!device->buttonPressed(SDLK_UP))
			{
				pressed = false;
				proccessed = false;
			}

			Camera2D *camera = RenderSystem::instance()->camera();

			float currentX = camera->currentScaleX();
			float currentY = camera->currentScaleY();
			float increase = 0.2f;

			if (pressed && !proccessed)
			{
				camera->setCurrentScale(currentX + increase, currentY + increase);
				proccessed = true;
			}
		}

		while (MessageSystem::instance()->pollMessage(message))
		{

			if (message)
			{
				if(message->type() == IMessage::MOVE)
				{
					MoveMessage *move = static_cast<MoveMessage*>(message);

					if(move->m_entityID == 0)
					{
						lineOfSight.start = move->m_newPosition;
					}
					else if(move->m_entityID == 1)
					{
						lineOfSight.end = move->m_newPosition;
					}
				}

				logic->processMessage(message);
				phys->processMessage(message);
				render->processMessage(message);
			}
		}
		
		Uint32 messageTicks = SDL_GetTicks() - ticks;

		logic->update();

		Uint32 logicTicks = SDL_GetTicks() - ticks;

		phys->update();

		render->clear();

		Uint32 physicsTicks = SDL_GetTicks() - ticks;

		render->update();

		Uint32 totalTicks = SDL_GetTicks() - ticks;

		if(counter % 200 == 0)
		{
			std::cout << "Message Ticks: " << messageTicks
				<< ", Logic: " << logicTicks
				<< ", Physics: " << physicsTicks
				<< " Total: " << totalTicks << std::endl;
		}

		counter++;
	}

	delete logic;
	delete input;
	delete phys;
	delete render;
	delete resource;
	delete init;

	return 0;
}