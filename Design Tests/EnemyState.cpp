#include "EnemyState.h"



EnemyState::EnemyState(int entityID, float weight)
	:IState(entityID), m_weight(weight)
{
}


EnemyState::~EnemyState()
{
}
