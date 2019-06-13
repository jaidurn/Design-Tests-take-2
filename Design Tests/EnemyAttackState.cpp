#include "EnemyAttackState.h"
#include "Rotation.h"

EnemyAttackState::EnemyAttackState(int entityID, float weight)
	:EnemyState(entityID, weight)
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
		std::string type = EntitySystem::instance()->entityType(m_entityID);

		if (type != "")
		{
			AttackInfo *attack = EntitySystem::instance()->entityAttack(m_entityID);

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

							int directionNumber = 4;
							float direction = angleToDegrees(position, targetPosition) / (360.0f / 4.0f);

							if (directionNumber <= direction)
							{
								direction = 0;
							}

							int realDirection = (int)round(direction);

							AnimationComponent::Direction dir = (AnimationComponent::Direction)realDirection;

							sendAnimationChangeMessage("Attack", dir, 0);
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
	m_timer.start();
}

void EnemyAttackState::update()
{
	if((int)m_timer.currentSeconds() % 5 == 0)
	{
		std::cout << "UPDATING ENEMY ATTACK STATE\n";
	}
}

void EnemyAttackState::exit()
{
	m_timer.stop();
}