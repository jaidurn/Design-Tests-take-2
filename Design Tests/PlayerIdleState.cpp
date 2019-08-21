#include "PlayerIdleState.h"
#include "MessageSystem.h"
#include "InputSystem.h"
#include "InputComponent.h"
#include "Controller.h"

PlayerIdleState::PlayerIdleState(int entityID, std::string name)
	:PlayerState(entityID, name)
{
}


PlayerIdleState::~PlayerIdleState()
{
}

//=============================================================================
// Function: void enter()
// Description:
// The logic to play upon the entrance of the state.
//=============================================================================
void PlayerIdleState::enter()
{
	// Start the animation for the entrance to the state here
	sendAnimationChangeMessage("Idle", AnimationComponent::DIR_NONE, 0);
}

//=============================================================================
// Function: void update()
// Description:
// Handles the update logic for the state.
//=============================================================================
void PlayerIdleState::update()
{
	InputComponent *input = InputSystem::instance()->getInputComponent(m_entityID);
	
	if (input)
	{
		if (input->xLeftAxis() != 0 || input->yLeftAxis() != 0)
		{
			sendStateChangeMessage("Walk");
		}
	}
}

//=============================================================================
// Function: void exit()
// Description:
// Does clean-up for exiting the state.
//=============================================================================
void PlayerIdleState::exit()
{
	// Add any functionality here.
}