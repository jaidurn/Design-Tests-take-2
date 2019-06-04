#include "PlayerIdleState.h"
#include "MessageSystem.h"
#include "InputSystem.h"
#include "InputComponent.h"
#include "Controller.h"

PlayerIdleState::PlayerIdleState(int entityID, std::string name)
	:IState(entityID), m_name(name)
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
}

//=============================================================================
// Function: void update()
// Description:
// Handles the update logic for the state.
//=============================================================================
void PlayerIdleState::update()
{
	Input::InputComponent *input = Input::InputSystem::instance()->getInputComponent(m_entityID);
	
	if (input)
	{
		if (input->xAxis() != 0 || input->yAxis() != 0)
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