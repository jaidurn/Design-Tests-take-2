#include "controller.h"
#include "MessageSystem.h"
#include "InputMessage.h"
#include <iostream>

Controller::Controller(int deviceID,
	int controllerID, 
	bool hapticEnabled)
	:InputDevice(deviceID, GAMEPAD),
	m_controller(NULL), 
	m_haptic(NULL), 
	m_ID(controllerID), 
	m_xLeftAxis(0.0f), 
	m_yLeftAxis(0.0f),
	m_xRightAxis(0.0f), 
	m_yRightAxis(0.0f),
	m_leftTriggerAxis(0.0f), 
	m_rightTriggerAxis(0.0f)
{
	open();
	if (hapticEnabled) { enableHaptic(); }
}

Controller::~Controller()
{
	close();
}

//=============================================================================
// Function: void mapButton(BUTTON_CODE, Button)
// Description:
// Maps a button code to a button.
// Parameters:
// BUTTON_CODE buttonCode - The button code to map.
// Button button - The button to map to it.
//=============================================================================
void Controller::mapButton(BUTTON_CODE buttonCode, Button button)
{
	auto mit = m_buttonMap.find(button);

	if (mit != m_buttonMap.end())
	{
		mit->second = buttonCode;
	}
	else
	{
		m_buttonMap.insert(std::make_pair(button, buttonCode));
	}
}

//=============================================================================
// Function: void pressButton(Button)
// Description:
// Finds the button code related to the button and sets it to pressed.
// Parameters:
// Button button - The button to press.
//=============================================================================
void Controller::pressButton(Button button)
{
	auto buttonMit = m_buttonMap.find(button);

	if (buttonMit != m_buttonMap.end())
	{
		BUTTON_CODE buttonCode = buttonMit->second;

		auto buttonStateMit = m_buttonState.find(buttonCode);

		if (buttonStateMit != m_buttonState.end())
		{
			buttonStateMit->second = true;

			sendInputButtonMsg(buttonCode, true);
		}
	}
}

//=============================================================================
// Function: void releaseButton(Button)
// Description:
// Finds the button code for the button and releases it.
// Parameters:
// Button button - The button to release.
//=============================================================================
void Controller::releaseButton(Button button)
{
	auto buttonMit = m_buttonMap.find(button);

	if (buttonMit != m_buttonMap.end())
	{
		BUTTON_CODE buttonCode = buttonMit->second;

		auto buttonStateMit = m_buttonState.find(buttonCode);

		if (buttonStateMit != m_buttonState.end())
		{
			buttonStateMit->second = false;

			sendInputButtonMsg(buttonCode, false);
		}
	}
}

//=============================================================================
// Function: bool buttonPressed(BUTTON_CODE)
// Description:
// Gets the pressed state of the buttonCode.
// Parameters:
// BUTTON_CODE buttonCode - The button code to check the state of.
// Output:
// bool 
// Returns true on button pressed 
// Returns false on button released.
//=============================================================================
bool Controller::buttonPressed(BUTTON_CODE buttonCode)
{
	bool pressed = false;

	auto mit = m_buttonState.find(buttonCode);

	if (mit != m_buttonState.end())
	{
		pressed = mit->second;
	}

	return pressed;
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

	sendInputAxisMsg(SDL_CONTROLLER_AXIS_LEFTX, m_xLeftAxis);
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

	sendInputAxisMsg(SDL_CONTROLLER_AXIS_LEFTY, m_yLeftAxis);
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

	sendInputAxisMsg(SDL_CONTROLLER_AXIS_RIGHTX, m_xRightAxis);
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

	sendInputAxisMsg(SDL_CONTROLLER_AXIS_RIGHTY, m_yRightAxis);
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

	sendInputAxisMsg(SDL_CONTROLLER_AXIS_TRIGGERLEFT, m_leftTriggerAxis);
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

	sendInputAxisMsg(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, m_rightTriggerAxis);
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
// Function: int getInstanceID()
// Description:
// Gets the instance ID of the controller.
// Output:
// int
// On success - Returns the instance id of the controller.
// On failure - Returns -1.
//=============================================================================
int Controller::getInstanceID()
{
	int instanceID = -1;

	SDL_Joystick *joystick = SDL_GameControllerGetJoystick(m_controller);

	if (joystick)
	{
		instanceID = SDL_JoystickInstanceID(joystick);
	}

	return instanceID;
}

//=============================================================================
// Function: void sendInputButtonMsg(BUTTON_CODE, bool)
// Description:
// Sends a message to the message system containing the button
// press information.
// Parameters:
// BUTTON_CODE buttonCode - The button code that was updated.
// bool pressed - The pressed state of the button.
//=============================================================================
void Controller::sendInputButtonMsg(BUTTON_CODE buttonCode, bool pressed)
{
	InputButtonMessage *button =
		new InputButtonMessage(m_deviceID, m_deviceType, buttonCode, pressed);

	MessageSystem::instance()->pushMessage(button);
}

//=============================================================================
// Function: void sendInputAxisMsg(Uint32, float)
// Description:
// Sends a message to the message system containing axis information.
// Parameters:
// Uint8 axis - The updated axis.
// float amount - The current axis amount.
//=============================================================================
void Controller::sendInputAxisMsg(Uint8 axis, float amount)
{
	InputAxisMessage *axisMsg =
		new InputAxisMessage(m_deviceID, m_deviceType, axis, amount);

	MessageSystem::instance()->pushMessage(axisMsg);
}

//=============================================================================
// Function: void initialize()
// Description:
// Initializes the controller with default button mapping.
//=============================================================================
void Controller::initialize()
{
	InputDevice::initialize();

	m_buttonMap.insert(std::make_pair(SDL_CONTROLLER_BUTTON_DPAD_UP, ANALOG_UP));
	m_buttonMap.insert(std::make_pair(SDL_CONTROLLER_BUTTON_DPAD_DOWN, ANALOG_DOWN));
	m_buttonMap.insert(std::make_pair(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, ANALOG_RIGHT));
	m_buttonMap.insert(std::make_pair(SDL_CONTROLLER_BUTTON_DPAD_LEFT, ANALOG_LEFT));
	m_buttonMap.insert(std::make_pair(SDL_CONTROLLER_BUTTON_A, SELECT));
	m_buttonMap.insert(std::make_pair(SDL_CONTROLLER_BUTTON_B, BACK));
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