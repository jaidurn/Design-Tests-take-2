#include "LogicSystem.h"
#include "MoveMessage.h"
#include "EntitySystem.h"
#include "PlayerLogicComponent.h"
#include "EnemyLogicComponent.h"
#include "EntityDestroyMessage.h"
#include "DoorLogicComponent.h"

LogicSystem::~LogicSystem()
{
	cleanUp();
}

//=============================================================================
// Function: LogicComponent* getLogicComponent(int)
// Description:
// Gets the logic component related to the entity ID.
// Parameters:
// int entityID - The id of the logic component.
// Output:
// LogicComponent* - The component that was found.
// Returns NULL on failure or if no component was found.
//=============================================================================
LogicComponent* LogicSystem::getLogicComponent(int entityID)
{
	LogicComponent *component = NULL;

	auto mit = m_logicComponents.find(entityID);

	if(mit != m_logicComponents.end())
	{
		component = mit->second;
	}

	return component;
}

//=============================================================================
// Function: LogicComponent* createLogicComponent(int, LogicType)
// Description:
// Creates a logic component for the id with the specified type.
// Parameters:
// int entityID - The id to create the component for.
// LogicType logic - The type of logic component to create.
// Output:
// LogicComponent* - The component that was created.
// Returns NULL on failure.
//=============================================================================
LogicComponent* LogicSystem::createLogicComponent(int entityID, LogicComponent::LogicType type)
{
	LogicComponent *component = NULL;

	auto mit = m_logicComponents.find(entityID);

	if(mit != m_logicComponents.end())
	{
		component = mit->second;
	}
	else
	{
		// Fill this in with the player and enemy logic components
		switch(type)
		{
		case LogicComponent::LOGIC_PLAYER:
		{
			component = new PlayerLogicComponent(LogicComponent::LOGIC_PLAYER, entityID);
			break;
		}
		case LogicComponent::LOGIC_ENEMY:
		{
			component = new EnemyLogicComponent(entityID);
			break;
		}
		case LogicComponent::LOGIC_DOOR:
		{
			component = new DoorLogicComponent(entityID, NULL);
			break;
		}
		}

		if(component)
		{
			m_logicComponents.insert(std::make_pair(entityID, component));
		}
	}

	return component;
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes the message passed to it.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void LogicSystem::processMessage(IMessage *message)
{
	if (message)
	{
		switch (message->type())
		{
		case IMessage::STATE_CHANGE:
		{
			StateChangeMessage *state = static_cast<StateChangeMessage*>(message);

			LogicComponent *component = getLogicComponent(state->m_entityID);

			if (component)
			{
				component->changeState(state->m_stateName);
			}

			break;
		}
		case IMessage::ENTITY_DESTROY:
		{
			EntityDestroyMessage *destroy = static_cast<EntityDestroyMessage*>(message);

			removeLogicComponent(destroy->m_entityID);

			break;
		}
		}

		// Send the messages to the logic components
		auto mit = m_logicComponents.begin();

		while(mit != m_logicComponents.end())
		{
			mit->second->processMessage(message);
			mit++;
		}
	}
}

//=============================================================================
// Function: void update()
// Description:
// Updates all of the logic components.
//=============================================================================
void LogicSystem::update(float delta)
{
	auto mit = m_logicComponents.begin();

	while(mit != m_logicComponents.end())
	{
		mit->second->update();
		mit++;
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Destroys all of the logic components, freeing the memory.
//=============================================================================
void LogicSystem::cleanUp()
{
	auto mit = m_logicComponents.begin();

	while(mit != m_logicComponents.end())
	{
		delete mit->second;
		mit = m_logicComponents.erase(mit);

		if(mit != m_logicComponents.end())
		{
			mit++;
		}
	}
}

//=============================================================================
// Function: void removeLogicComponent(int)
// Description:
// Finds and removes the logic component for the entity ID.
// Parameters:
// int entityID - The ID of the logic component to remove.
//=============================================================================
void LogicSystem::removeLogicComponent(int entityID)
{
	auto mit = m_logicComponents.find(entityID);

	if(mit != m_logicComponents.end())
	{
		delete mit->second;

		mit = m_logicComponents.erase(mit);
	}
}