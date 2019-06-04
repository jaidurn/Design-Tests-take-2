#include "EnemyIdleState.h"



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
bool EnemyIdleState::canEnter()
{
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