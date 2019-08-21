#include "EnemyTargetState.h"
#include "CollisionMessage.h"


EnemyTargetState::EnemyTargetState(int entityID, float weight, bool lineOfSight, std::string targetType, int range)
	:EnemyState(entityID, weight), m_lineOfSight(lineOfSight), m_targetType(targetType), m_currentTarget(-1), m_range(range)
{

}


EnemyTargetState::~EnemyTargetState()
{

}

bool EnemyTargetState::canEnter(int targetID, Behavior behavior)
{
	if (targetID != -1)
	{
		CollisionComponent *target = PhysicsSystem::instance()->getCollisionComponent(targetID);
		CollisionComponent *current = PhysicsSystem::instance()->getCollisionComponent(m_entityID);

		// Just keep our current target if it's in range
		if (totalDistance(target->center(), current->center()) <= m_range)
		{
			float distance = totalDistance(target->center(), current->center());

			return false;
		}
	}

	return true;
}

bool EnemyTargetState::canExit()
{
	return true;
}

void EnemyTargetState::enter()
{

}

void EnemyTargetState::update()
{
	if(m_currentTarget != -1)
	{
		CollisionComponent *target = PhysicsSystem::instance()->getCollisionComponent(m_currentTarget);

		if(target)
		{
			CollisionComponent *self = PhysicsSystem::instance()->getCollisionComponent(m_entityID);

			if (totalDistance(target->center(), self->center()) <= m_range)
			{

			}
			else
			{
				std::cout << "Entity: " << m_entityID << " Target lost!\n";
				m_currentTarget = -1;
			}
		}
	}
	else
	{
		if (PhysicsSystem::instance()->collisionSystem()->circleCollision(m_entityID, m_range))
		{
			IMessage *message = NULL;

			bool targetFound = false;

			for(int i = 0; i < MessageSystem::instance()->messageCount(); i++)
			{
				if (MessageSystem::instance()->peekMessage(message, i))
				{
					if (message->type() == IMessage::COLLISION)
					{
						CollisionMessage *collision = static_cast<CollisionMessage*>(message);

						if (EntitySystem::instance()->entityType(collision->m_collidingID) == m_targetType)
						{
							if (m_currentTarget == -1)
							{
								if (m_lineOfSight)
								{
									if (PhysicsSystem::instance()->hasLineOfSight(m_entityID, collision->m_collidingID))
									{
										m_currentTarget = collision->m_collidingID;
										targetFound = true;
									}
								}
								else
								{
									CollisionComponent *target = PhysicsSystem::instance()->getCollisionComponent(collision->m_collidingID);
									CollisionComponent *self = PhysicsSystem::instance()->getCollisionComponent(m_entityID);

									if (target && self)
									{
										if (totalDistance(target->center(), self->center()) <= m_range)
										{
											m_currentTarget = collision->m_collidingID;
											targetFound = true;
										}
									}
								}
							}
							else
							{
								CollisionComponent *compA = PhysicsSystem::instance()->getCollisionComponent(m_entityID);
								CollisionComponent *compB = PhysicsSystem::instance()->getCollisionComponent(m_currentTarget);
								CollisionComponent *compC = PhysicsSystem::instance()->getCollisionComponent(collision->m_collidingID);

								if (totalDistance(compA->center(), compC->center()) < totalDistance(compA->center(), compB->center()))
								{
									if (m_lineOfSight)
									{
										if (PhysicsSystem::instance()->hasLineOfSight(m_entityID, collision->m_collidingID))
										{
											targetFound = true;
											m_currentTarget = collision->m_collidingID;
										}
									}
									else
									{
										targetFound = true;
										m_currentTarget = collision->m_collidingID;
									}
								}
							}
						}
					}
				}
			}

			if (!targetFound)
			{
				if(m_currentTarget != -1)
				{
					std::cout << "Entity: " << m_entityID << "Target lost!\n";
				}

				m_currentTarget = -1;
			}
			else
			{
				std::cout << "Entity: " << m_entityID << " CURRENT TARGET: " << m_currentTarget << std::endl;
			}
		}
	}
}

void EnemyTargetState::exit()
{

}