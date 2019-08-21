#include "controller.h"
#include <iostream>

using namespace Input;

Controller::Controller(int controllerID, bool hapticEnabled)
	:m_controller(NULL), m_haptic(NULL), m_ID(controllerID), m_xLeftAxis(0.0f), m_yLeftAxis(0.0f),
	m_xRightAxis(0.0f), m_yRightAxis(0.0f),
	m_leftTriggerAxis(0.0f), m_rightTriggerAxis(0.0f)
{
	open();
	if (hapticEnabled) { enableHaptic(); }
}


Controller::~Controller()
{
	close();
}


//=============================================================================
// Function: void pressButton(Uint8)
// Description:
// Toggles the button in the map to pressed.
// Parameters:
// Uint8 button - The button ID to press.
//=============================================================================
void Controller::pressButton(Button button)
{
	m_buttons[button] = true;
}

//=============================================================================
// Function: void releaseButton(Uint8)
// Description:
// Releases the button.
// Parameters:
// Uint8 button - The button ID to release.
//=============================================================================
void Controller::releaseButton(Button button)
{
	m_buttons[button] = false;
}

//=============================================================================
// Function: void moveLeftAxisX(Sint16)
// Description:
// Sets the left x axis to the movement amount.
// Parameters:
// Sint16 movement - How much to move our x axis.
//=============================================================================
void Controller::moveLeftAxisX(Sint16 movement)
{
	float moveAmount = 0.0f;

	int workingDeadZone = m_DEAD_ZONE;
	float workingAxisMax = (float)(m_AXIS_MAX);
	float workingMovement = (float)(movement);

	if (movement < 0.0f) 
	{ 
		workingDeadZone *= -1;

		if (movement < workingDeadZone)
		{
			moveAmount = workingMovement / workingAxisMax;
		}
	}
	else if (0.0f < movement) 
	{ 
		if (workingDeadZone < movement)
		{
			moveAmount = workingMovement / (workingAxisMax - 1);
		}
	}

	m_xLeftAxis = moveAmount;
}

//=============================================================================
// Function: void moveLeftAxisY(Sint16)
// Description:
// Sets the left y axis to the movement amount.
// Parameters:
// Sint16 movement - How much to move our y axis.
//=============================================================================
void Controller::moveLeftAxisY(Sint16 movement)
{
	float moveAmount = 0.0f;

	int workingDeadZone = m_DEAD_ZONE;
	float workingAxisMax = (float)(m_AXIS_MAX);
	float workingMovement = (float)(movement);

	if (movement < 0.0f) 
	{ 
		workingDeadZone *= -1;
		
		if (workingMovement < workingDeadZone)
		{
			moveAmount = workingMovement / workingAxisMax;
		}
	}
	else if (0.0f < movement) 
	{ 
		if (workingDeadZone < workingMovement)
		{
			moveAmount = workingMovement / (workingAxisMax - 1);
		}
	}

	m_yLeftAxis = moveAmount;
}

//=============================================================================
// Function: void moveRightAxisX(Sint16)
// Description:
// Sets the x axis to the movement amount.
// Parameters:
// Sint16 movement - How much to move our x axis.
//=============================================================================
void Controller::moveRightAxisX(Sint16 movement)
{
	float moveAmount = 0.0f;

	int workingDeadZone = m_DEAD_ZONE;
	float workingAxisMax = (float)(m_AXIS_MAX);
	float workingMovement = (float)(movement);

	if (movement < 0.0f)
	{
		workingDeadZone *= -1;

		if (movement < workingDeadZone)
		{
			moveAmount = workingMovement / workingAxisMax;
		}
	}
	else if (0.0f < movement)
	{
		if (workingDeadZone < movement)
		{
			moveAmount = workingMovement / (workingAxisMax - 1);
		}
	}

	m_xRightAxis = moveAmount;
}

//=============================================================================
// Function: void moveRightAxisY(Sint16)
// Description:
// Sets the y axis to the movement amount.
// Parameters:
// Sint16 movement - How much to move our y axis.
//=============================================================================
void Controller::moveRightAxisY(Sint16 movement)
{
	float moveAmount = 0.0f;

	int workingDeadZone = m_DEAD_ZONE;
	float workingAxisMax = (float)(m_AXIS_MAX);
	float workingMovement = (float)(movement);

	if (movement < 0.0f)
	{
		workingDeadZone *= -1;

		if (workingMovement < workingDeadZone)
		{
			moveAmount = workingMovement / workingAxisMax;
		}
	}
	else if (0.0f < movement)
	{
		if (workingDeadZone < workingMovement)
		{
			moveAmount = workingMovement / (workingAxisMax - 1);
		}
	}

	m_yRightAxis = moveAmount;
}

//=============================================================================
// Function: void moveLeftTriggerAxis(Sint16)
// Description:
// Sets the left trigger axis to the movement amount.
// Parameters:
// Sint16 movement - How much to move our trigger axis.
//=============================================================================
void Controller::moveLeftTriggerAxis(Sint16 movement)
{
	float moveAmount = 0.0f;

	float workingAxisMax = (float)(m_AXIS_MAX);
	float workingMovement = (float)(movement);

	moveAmount = workingMovement / (workingAxisMax - 1);

	m_leftTriggerAxis = moveAmount;
}

//=============================================================================
// Function: void moveRightTriggerAxis(Sint16)
// Description:
// Sets the right trigger axis to the movement amount.
// Parameters:
// Sint16 movement - How much to move our trigger axis.
//=============================================================================
void Controller::moveRightTriggerAxis(Sint16 movement)
{
	float moveAmount = 0.0f;

	float workingAxisMax = (float)(m_AXIS_MAX);
	float workingMovement = (float)(movement);

	moveAmount = workingMovement / (workingAxisMax - 1);

	m_rightTriggerAxis = moveAmount;
}

//=============================================================================
// Function: void triggerHaptic(float, Uint32)
// Description:
// Triggers the haptic for the device if it's enabled with a specified
// strength and length.
// Parameters:
// float strength - The strength of the feed back. 0.0 - 1.0.
// Uint32 length - The number of ticks to trigger the haptic for.
//=============================================================================
void Controller::triggerHaptic(float strength, Uint32 length)
{
	if(m_haptic)
	{
		SDL_HapticRumblePlay(m_haptic, strength, length);
	}
}

//=============================================================================
// Function: bool buttonPressed(Uint8 button)
// Description:
// Checks if the button is pressed. If the button doesn't exist in our map,
// then that means it's not pressed.
// Parameters:
// Uint8 button - The button id to check.
// Output:
// bool - Returns true on button pressed, false on button released.
//=============================================================================
bool Controller::buttonPressed(Button button)
{
	auto mit = m_buttons.find(button);

	if (mit == m_buttons.end())
	{
		return false;
	}

	return mit->second;
}

//=============================================================================
// Function: void toggleHapticOnOff()
// Description:
// Toggles the haptic feedback on and off. Use for user preferences.
//=============================================================================
void Controller::toggleHapticOnOff()
{
	if (m_haptic) { disableHaptic(); }
	else { enableHaptic(); }
}

//=============================================================================
// Function: void open()
// Description:
// Opens the SDL_GameController.
//=============================================================================
void Controller::open()
{
	m_controller = SDL_GameControllerOpen(m_ID);

	if(!m_controller)
	{
		std::cout << "ERROR CREATING JOYSTICK WITH ID: " << m_ID << std::endl;
	}
}

//=============================================================================
// Function: void enableHaptic()
// Description:
// Enables the haptic feature of the controller.
//=============================================================================
void Controller::enableHaptic()
{
	if (m_haptic == NULL)
	{
		m_haptic = SDL_HapticOpen(m_ID);

		SDL_HapticRumbleInit(m_haptic);

		if (m_haptic == NULL)
		{
			std::cout << "ERROR ENABLING HAPTIC FOR CONTROLLER WITH ID: " << m_ID << std::endl;
		}
	}
}

//=============================================================================
// Function: void disableHaptic()
// Description:
// Disables haptic feedback for the controller.
//=============================================================================
void Controller::disableHaptic()
{
	if (m_haptic)
	{
		SDL_HapticClose(m_haptic);

		m_haptic = NULL;
	}
}

//=============================================================================
// Function: void close()
// Description:
// Closes the controller and frees it up. If haptic is on, it cleans that up, too.
//=============================================================================
void Controller::close()
{	
	if (m_haptic) { disableHaptic(); }
	
	if (m_controller)
	{
		SDL_GameControllerClose(m_controller);
		m_controller = NULL;
	}
}