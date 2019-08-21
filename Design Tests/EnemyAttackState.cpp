#include "EnemyAttackState.h"
#include "Rotation.h"
#include "CollisionMessage.h"
#include "EntityDestroyMessage.h"
#include "VelocityIncreaseMessage.h"

EnemyAttackState::EnemyAttackState(int entityID, float weight)
	:EnemyState(entityID, weight), m_attacked(false), m_currentTarget(-1), m_currentAttack(-1)
{

}

EnemyAttackState::~EnemyAttackState()
{

}

bool EnemyAttackState::canEnter(int targetID, Behavior behavior)
{
	bool readyToEnter = false;

	if(targetID != -1)
	{
		if (m_attacked)
		{
			if ((int)m_cooldownTimer.currentSeconds() % 2 == 0 && (int)m_cooldownTimer.currentSeconds() != 0)
			{
				int key = EntitySystem::instance()->getEntityKey(m_entityID) + 1;

				AttackInfo *attack = EntitySystem::instance()->entityAttack(key);

				if (attack)
				{
					CollisionComponent *collision = PhysicsSystem::instance()->getCollisionComponent(m_entityID);

					if (collision)
					{
						Vector2D position = collision->center();

						CollisionComponent *targetCollision = PhysicsSystem::instance()->getCollisionComponent(targetID);

						if (targetCollision)
						{
							Vector2D targetPosition = targetCollision->center();

							float attackRangeModifier = 1.0f;

							switch (behavior)
							{
							case BEHAVIOR_NONE:
								break;
							case BEHAVIOR_AGGRESSIVE:
								attackRangeModifier += .2f;
								break;
							case BEHAVIOR_NORMAL:
								break;
							case BEHAVIOR_PASSIVE:
								attackRangeModifier -= .2f;
								break;
							}

							if ((float)round(totalDistance(position, targetPosition)) <= (float)attack->range() * attackRangeModifier)
							{
								readyToEnter = true;

								// Look in the player's direction.
								float f_direction = angleToDegrees(collision->center(), targetCollision->center());
								int directionCount = (int)AnimationComponent::DIR_MAX;
								float directionDegrees = f_direction / (360.0f / (float)directionCount);

								int i_direction = (int)(round(directionDegrees));

								AnimationComponent::Direction direction = (AnimationComponent::Direction)i_direction;

								if (AnimationComponent::DIR_MAX == direction)
								{
									direction = AnimationComponent::DIR_DOWN;
								}

								sendAnimationChangeMessage("Attack", direction, 0);

								m_currentAttack = EntitySystem::instance()->createEntity(key);
								m_currentTarget = targetID;

								if (m_currentAttack == -1)
								{
									std::cout << "Error creating an attack for key: " << key << std::endl;

									readyToEnter = false;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			int key = EntitySystem::instance()->getEntityKey(m_entityID) + 1;

			AttackInfo *attack = EntitySystem::instance()->entityAttack(key);

			if (attack)
			{
				CollisionComponent *collision = PhysicsSystem::instance()->getCollisionComponent(m_entityID);

				if (collision)
				{
					Vector2D position = collision->center();

					CollisionComponent *targetCollision = PhysicsSystem::instance()->getCollisionComponent(targetID);

					if (targetCollision)
					{
						Vector2D targetPosition = targetCollision->center();

						float attackRangeModifier = 1.0f;

						switch (behavior)
						{
						case BEHAVIOR_NONE:
							break;
						case BEHAVIOR_AGGRESSIVE:
							attackRangeModifier += .2f;
							break;
						case BEHAVIOR_NORMAL:
							break;
						case BEHAVIOR_PASSIVE:
							attackRangeModifier -= .2f;
							break;
						}

						if ((float)round(totalDistance(position, targetPosition)) <= (float)attack->range() * attackRangeModifier)
						{
							readyToEnter = true;

							// Look in the player's direction.
							float f_direction = angleToDegrees(collision->center(), targetCollision->center());
							int directionCount = (int)AnimationComponent::DIR_MAX;
							float directionDegrees = f_direction / (360.0f / (float)directionCount);

							int i_direction = (int)(round(directionDegrees));

							AnimationComponent::Direction direction = (AnimationComponent::Direction)i_direction;

							if (AnimationComponent::DIR_MAX == direction)
							{
								direction = AnimationComponent::DIR_DOWN;
							}

							sendAnimationChangeMessage("Attack", direction, 0);

							m_currentAttack = EntitySystem::instance()->createEntity(key);
							m_currentTarget = targetID;

							if(m_currentAttack == -1)
							{
								std::cout << "Error creating an attack for key: " << key << std::endl;

								readyToEnter = false;
							}
						}
					}
				}
			}
		}
	}

	return readyToEnter;
}

bool EnemyAttackState::canExit()
{
	bool canExit = false;

	AnimationComponent *animation = RenderSystem::instance()->getAnimation(m_entityID);

	if(animation)
	{
		if (animation->currentFrameIndex() == animation->animationLength("Attack") - 1)
		{
			canExit = true;
		}
	}

	return canExit;
}

void EnemyAttackState::enter()
{
	m_attacked = false;
	m_cooldownTimer.stop();
	m_timer.start();
}

void EnemyAttackState::update()
{
	int currentSecond = (int)m_timer.currentSeconds();

	if (m_currentAttack != -1)
	{
		int key = EntitySystem::instance()->getEntityKey(m_currentAttack);
		AttackInfo *attack = EntitySystem::instance()->entityAttack(key);

		int maskCount = attack->maskCount();
		int maskIndex = (int)RenderSystem::instance()->getAnimation(m_entityID)->currentFrameIndex();
		int direction = (int)RenderSystem::instance()->getAnimation(m_entityID)->currentDirection();
		float rotationAmount = 100.0f / (float)AnimationComponent::DIR_MAX;

		CollisionSystem *collisionSys = PhysicsSystem::instance()->collisionSystem();
		CollisionComponent *selfCollision = PhysicsSystem::instance()->getCollisionComponent(m_entityID);

		for(int i = 0; i < maskCount; i++)
		{
			pRectangle mask = attack->getCollisionBox(i, maskIndex);
			
			Vector2D position = mask->center() + selfCollision->center();
			
			position = rotatePoint(position, selfCollision->center(), -(direction * rotationAmount));

			collisionSys->squareCollision(m_currentAttack, (int)position.getX(), (int)position.getY(), mask->width(), mask->height());
		}
	}

	if((int)m_timer.currentSeconds() != 0 && (int)m_timer.currentSeconds() % 5 == 0)
	{
		std::cout << "Current Second: " << currentSecond << std::endl;
		std::cout << "UPDATING ENEMY ATTACK STATE\n";
	}
}

void EnemyAttackState::exit()
{
	if(m_currentAttack != -1)
	{
		EntityDestroyMessage *destroy = new EntityDestroyMessage(m_currentAttack);

		MessageSystem::instance()->pushMessage(destroy);
	}

	m_attacked = true;
	m_cooldownTimer.start();
	m_timer.stop();
}