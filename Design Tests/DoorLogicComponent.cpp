#include "DoorLogicComponent.h"
#include "CollisionMessage.h"
#include "Room.h"
#include "EntitySystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"

DoorLogicComponent::DoorLogicComponent(int entityID, Door *door)
	:LogicComponent(LOGIC_DOOR, entityID), m_door(door), m_currentState(DOOR_CLOSED)
{
	RenderSystem *sysRender = RenderSystem::instance();
	
	AnimationComponent *animation = sysRender->getAnimation(entityID);

	if(animation)
	{
		animation->setAnimation("Idle");
		animation->setCurrentDirection(AnimationComponent::DIR_DOWN);
	}
}

DoorLogicComponent::~DoorLogicComponent()
{
	m_door = NULL;
}

void DoorLogicComponent::update()
{
	if(m_door)
	{
		switch(m_currentState)
		{
		case DOOR_CLOSED:
		{
			break;
		}
		case DOOR_LOCKED:
		{
			break;
		}
		default:
		{
			// Do nothing.
			break;
		}
		}
	}
}

void DoorLogicComponent::processMessage(IMessage *message)
{
	if(m_door)
	{
		switch (m_currentState)
		{
		case DOOR_CLOSED:
		{
			switch(message->type())
			{
			case IMessage::COLLISION:
			{
				CollisionMessage *collision = static_cast<CollisionMessage*>(message);

				// If one of the entities is the door
				if(collision->m_entityID == m_entityID || collision->m_collidingID == m_entityID)
				{
					// Check if the other is a player
				
					EntitySystem *sysEntity = EntitySystem::instance();

					if(sysEntity->entityType(collision->m_entityID) == "Player" ||
						sysEntity->entityType(collision->m_collidingID) == "Player")
					{
						openDoor();
					}
				}

				break;
			}
			}
			break;
		}
		case DOOR_LOCKED:
		{
			break;
		}
		default:
		{
			// By default, do nothing.
			break;
		}
		}
	}
}

void DoorLogicComponent::setDoor(Door *door)
{
	if(door)
	{
		m_door = door;
	}
}

void DoorLogicComponent::openDoor()
{
	if (m_door)
	{
		m_door->m_closed = false;

		m_currentState = DOOR_OPEN;

		RenderSystem *sysRender = RenderSystem::instance();

		AnimationComponent *animation = sysRender->getAnimation(m_entityID);

		if (animation)
		{
			animation->setCurrentFrame(1);
		}

		PhysicsSystem *sysPhysics = PhysicsSystem::instance();

		CollisionComponent *collision = sysPhysics->getCollisionComponent(m_entityID);

		if (collision)
		{
			collision->setSolid(false);
		}
	}
}