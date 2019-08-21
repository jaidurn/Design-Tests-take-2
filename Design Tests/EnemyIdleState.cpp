#include "EnemyIdleState.h"
#include "Rotation.h"


EnemyIdleState::EnemyIdleState(int entityID, float weight)
	:EnemyState(entityID, weight)
{
}


EnemyIdleState::~EnemyIdleState()
{
}

//=============================================================================
// Function: bool canEnter()
// Description:
// Checks if the state is able to be entered.
// Output:
// Returns true if the state can be entered.
//=============================================================================
bool EnemyIdleState::canEnter(int targetID, Behavior behavior)
{
	if(targetID != -1)
	{
		CollisionComponent *target = PhysicsSystem::instance()->getCollisionComponent(targetID);
		CollisionComponent *self = PhysicsSystem::instance()->getCollisionComponent(m_entityID);

		if (target && self)
		{
			// Look in the player's direction.
			float f_direction = angleToDegrees(self->center(), target->center());
			int directionCount = (int)AnimationComponent::DIR_MAX;
			float directionDegrees = f_direction / (360.0f / (float)directionCount);

			int i_direction = (int)(round(directionDegrees));

			AnimationComponent::Direction direction = (AnimationComponent::Direction)i_direction;

			if(AnimationComponent::DIR_MAX == direction)
			{
				direction = AnimationComponent::DIR_DOWN;
			}

			sendAnimationChangeMessage("Idle", direction, 0);
		}
	}

	return true; 
}

//=============================================================================
// Function: bool canExit()
// Description:
// Checks if the state is currently able to be exited.
// Output:
// Returns true if this state is ready to be exited.
//=============================================================================
bool EnemyIdleState::canExit()
{
	return true;
}

//=============================================================================
// Function: void enter()
// Description:
// Handles the logic required to start the state.
//=============================================================================
void EnemyIdleState::enter()
{
	m_timer.start();
}

//=============================================================================
// Function: void update()
// Description:
// Handles the logic of the state.
//=============================================================================
void EnemyIdleState::update()
{
	// Literally do nothing for now.
}

//=============================================================================
// Function: void exit()
// Description:
// Handles the logic required to exit the state.
//=============================================================================
void EnemyIdleState::exit()
{
	m_timer.stop();
}