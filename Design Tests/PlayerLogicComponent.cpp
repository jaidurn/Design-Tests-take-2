#include "PlayerLogicComponent.h"
#include "CollisionMessage.h"
#include "EntitySystem.h"
#include "AttackInfo.h"
#include "PhysicsSystem.h"
#include "CollisionComponent.h"
#include "VelocityIncreaseMessage.h"
#include "RenderSystem.h"

PlayerLogicComponent::~PlayerLogicComponent()
{
	cleanUp();
}

//=============================================================================
// Function: void addState(string, IState*)
// Description:
// Adds a state to the list of states if one doesn't already exist.
// Parameters:
// string stateName - The name to create the state with.
// IState *state - The state to add to the states.
//=============================================================================
void PlayerLogicComponent::addState(std::string stateName, IState *state)
{
	if (stateName != "")
	{
		auto mit = m_states.find(stateName);

		if (mit == m_states.end())
		{
			m_states.insert(std::make_pair(stateName, state));
		}
		else
		{
			// Clean up the state.
			delete state;
		}
	}
	else
	{
		delete state;
	}
}

//=============================================================================
// Function: void removeState(string)
// Description:
// Removes the state with the specified name.
// Parameters:
// string stateName - The name of the state to remove.
//=============================================================================
void PlayerLogicComponent::removeState(std::string stateName)
{
	if (stateName != "")
	{
		auto mit = m_states.find(stateName);

		if (mit != m_states.end())
		{
			delete mit->second;
			mit = m_states.erase(mit);
		}
	}
}

//=============================================================================
// Function: IState* getState(string)
// Description:
// Gets the state information related to the state name.
// Parameters:
// string stateName - The name of the state to get.
// Output:
// IState* - Returns the state found.
// Returns NULL on failure, or if no state was found.
//=============================================================================
IState* PlayerLogicComponent::getState(std::string stateName)
{
	IState *state = NULL;

	if (stateName != "")
	{
		auto mit = m_states.find(stateName);

		if (mit != m_states.end())
		{
			state = mit->second;
		}
	}

	return state;
}

//=============================================================================
// Function: IState* currentState()
// Description:
// Gets the information for the current state.
// Output:
// IState* - Returns the current state.
// Returns NULL if the current state isn't set.
//=============================================================================
IState* PlayerLogicComponent::currentState()
{
	return m_currentState;
}

//=============================================================================
// Function: void changeState(string)
// Description:
// Changes the state to the specified state if it exists.
// Parameters:
// string stateName - The state name to switch to.
//=============================================================================
void PlayerLogicComponent::changeState(std::string stateName)
{
	if (stateName != "")
	{
		if (stateName != m_currentStateName)
		{
			if (getState(stateName) != NULL)
			{
				m_currentState = getState(stateName);

				m_currentState->enter();

				m_currentStateName = stateName;
			}
		}
	}
}

//=============================================================================
// Function: void update()
// Description:
// If there's a current state, it handles the logic behind it.
//=============================================================================
void PlayerLogicComponent::update()
{
	if(m_currentState)
	{
		m_currentState->update();
	}
}

//=============================================================================
// Function: void processMessage(IMessage *message)
// Description:
// Processes system messages passed in.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void PlayerLogicComponent::processMessage(IMessage *message)
{
	if(message)
	{
		switch(message->type())
		{
		case IMessage::COLLISION:
		{
			CollisionMessage *collision = static_cast<CollisionMessage*>(message);

			if(collision->m_collidingID == m_entityID)
			{
				if (EntitySystem::instance()->entityType(collision->m_entityID) == "EnemyAttack")
				{
					if (canKnockback())
					{
						int attackKey = EntitySystem::instance()->getEntityKey(collision->m_entityID);

						if (attackKey != -1)
						{
							AttackInfo *attack = EntitySystem::instance()->entityAttack(attackKey);

							if (attack)
							{
								CollisionComponent *selfCol = PhysicsSystem::instance()->getCollisionComponent(m_entityID);

								if (selfCol)
								{
									Vector2D distance = collision->m_position - selfCol->center();

									float angle = (float)atan2(distance.getY(), distance.getX());

									float xVel = (float)(cos(angle + M_PI) * attack->knockback());
									float yVel = (float)(sin(angle + M_PI) * attack->knockback());

									VelocityIncreaseMessage *vel = new VelocityIncreaseMessage(m_entityID, xVel, yVel, attack->knockback(), attack->knockback());

									MessageSystem::instance()->pushMessage(vel);

									SDL_Color redFade{ 255, 40, 40, 100 };

									RenderSystem::instance()->createTextureEffect(m_entityID, TextureEffect::EFFECT_ALL_FLASH, redFade, SDL_BLENDMODE_BLEND, m_knockbackCooldown, 12.0f);

									m_knockback.start();
								}
							}
						}
					}
				}
			}
				
			break;
		}
		}

		if(m_currentState)
		{
			m_currentState->processMessage(message);
		}
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up all of the states.
//=============================================================================
void PlayerLogicComponent::cleanUp()
{
	auto mit = m_states.begin();

	while(mit != m_states.end())
	{
		delete mit->second;

		mit = m_states.erase(mit);

		if(mit != m_states.end())
		{
			mit++;
		}
	}
}

bool PlayerLogicComponent::canKnockback()
{
	bool knockbackAllowed = false;

	if(m_knockback.running() == true)
	{
		if(m_knockbackCooldown <= m_knockback.currentSeconds())
		{
			knockbackAllowed = true;

			m_knockback.stop();
		}
	}
	else
	{
		knockbackAllowed = true;
	}

	return knockbackAllowed;
}