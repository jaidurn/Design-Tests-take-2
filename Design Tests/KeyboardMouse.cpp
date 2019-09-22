#include "KeyboardMouse.h"
#include "MessageSystem.h"
#include "InputMessage.h"

KeyboardMouse::KeyboardMouse(int deviceID)
	:InputDevice(deviceID, KEYBOARD_MOUSE),
	m_position(0,0)
{
	initialize();
}

KeyboardMouse::~KeyboardMouse()
{

}

//=============================================================================
// Function: void mapButton(BUTTON_CODE, Button)
// Description:
// Maps a button code to a button.
// Parameters:
// BUTTON_CODE buttonCode - The button code to map.
// Button button - The button to map to it.
//=============================================================================
void KeyboardMouse::mapButton(BUTTON_CODE buttonCode, Button button)
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
// Finds the buttonCode mapped to the button and presses it.
// Parameters:
// Button button - The button to press.
//=============================================================================
void KeyboardMouse::pressButton(Button button)
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
// Finds the buttonCode mapped to the button and releases it.
// Parameters:
// Button button - The SDL button to release.
//=============================================================================
void KeyboardMouse::releaseButton(Button button)
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
// Gets the current state of the specified button code.
// Parameters:
// BUTTON_CODE buttonCode - The button code to get the state of.
// Output:
// bool
// Returns true if the button is pressed.
// Returns false if the button isn't pressed.
//=============================================================================
bool KeyboardMouse::buttonPressed(BUTTON_CODE buttonCode)
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
// Function: Vector2D getPosition()
// Description:
// Gets the position of the mouse.
// Output:
// Vector2D
// Returns a Vector2D object containing the position of the mouse.
//=============================================================================
Vector2D KeyboardMouse::getPosition()
{
	return m_position;
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the mouse position.
// Parameters:
// Vector2D position - The new mouse position.
//=============================================================================
void KeyboardMouse::setPosition(Vector2D position)
{
	m_position = position;

	sendInputMoveMsg();
}

//=============================================================================
// Function: void setPosition(int, int)
// Description:
// Sets the mouse position.
// Parameters:
// int x - The new x position of the mouse.
// int y - The new y position of the mouse.
//=============================================================================
void KeyboardMouse::setPosition(int x, int y)
{
	m_position.setX((float)x);
	m_position.setY((float)y);

	sendInputMoveMsg();
}

//=============================================================================
// Function: void initialize()
// Description:
// Initializes all of the button codes with default values.
// TODO: Make it so they get loaded in by the input system.
//=============================================================================
void KeyboardMouse::initialize()
{
	InputDevice::initialize();

	m_buttonMap.insert(std::make_pair(SDLK_UP, ANALOG_UP));
	m_buttonMap.insert(std::make_pair(SDLK_DOWN, ANALOG_DOWN));
	m_buttonMap.insert(std::make_pair(SDLK_RIGHT, ANALOG_RIGHT));
	m_buttonMap.insert(std::make_pair(SDLK_LEFT, ANALOG_LEFT));
	m_buttonMap.insert(std::make_pair(SDL_BUTTON_LEFT, SELECT));
	m_buttonMap.insert(std::make_pair(SDLK_KP_ENTER, SELECT));
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
void KeyboardMouse::sendInputButtonMsg(BUTTON_CODE buttonCode, bool pressed)
{
	InputButtonMessage *button = 
		new InputButtonMessage(m_deviceID, m_deviceType, buttonCode, pressed);

	MessageSystem::instance()->pushMessage(button);
}

//=============================================================================
// Function: void sendInputMoveMsg()
// Description:
// Sends a message to the message system containing the
// mouse movement information.
//=============================================================================
void KeyboardMouse::sendInputMoveMsg()
{
	InputMoveMessage *move =
		new InputMoveMessage(m_deviceID, m_deviceType, m_position.getX(), m_position.getY());

	MessageSystem::instance()->pushMessage(move);
}