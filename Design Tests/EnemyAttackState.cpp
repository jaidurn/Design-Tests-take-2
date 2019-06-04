#include "EnemyAttackState.h"

EnemyAttackState::EnemyAttackState(int entityID, float weight)
	:EnemyState(entityID, weight)
{

}

EnemyAttackState::~EnemyAttackState()
{

}

bool EnemyAttackState::canEnter(Behavior behavior)
{
	bool readyToEnter = false;

	std::string type = EntitySystem::instance()->entityType(m_entityID);

	if(type != "")
	{
		AttackInfo *attack = EntitySystem::instance()->entityAttack(m_entityID);

		if(attack)
		{

		}
	}

	return readyToEnter;
}