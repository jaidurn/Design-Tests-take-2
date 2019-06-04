#include "InputComponent.h"
#include "Controller.h"
#include "Keyboard.h"
#include "Rotation.h"
#include <iostream>

using namespace Input;

InputComponent::InputComponent()
	:Component(INPUT)
{
	m_devices.resize(0);
}

InputComponent::~InputComponent()
{
	// Make sure to free the devices
	freeDevices();
}

int InputComponent::numberOfDevices()
{
	int deviceCount = 0;

	deviceCount = (int)(m_devices.size());

	return deviceCount;
}

InputDevice* InputComponent::getDevice(int deviceNumber)
{
	InputDevice *device = NULL;

	if(deviceNumber <= numberOfDevices())
	{
		if(0 <= deviceNumber)
		{
			device = m_devices[deviceNumber];
		}
	}

	return device;
}

void InputComponent::addDevice(InputDevice *device)
{
	m_devices.push_back(device);
}

InputDevice::DEVICE_TYPE InputComponent::getDeviceType(int deviceNumber)
{
	InputDevice::DEVICE_TYPE type = InputDevice::DEVICE_TYPE::NONE;

	if(deviceNumber <= numberOfDevices())
	{
		if(0 <= deviceNumber)
		{
			type = m_devices[deviceNumber]->type();
		}
	}

	return type;
}

//=============================================================================
// Function: bool buttonPressed(Button)
// Description:
// Checks all of the input devices to see if the button is pressed.
// Parameters:
// Button button - The button to check if it's pressed.
// Output:
// Returns true if the button is pressed, false if it isn't.
//=============================================================================
bool InputComponent::buttonPressed(Button button)
{
	bool pressed = false;

	for(int i = 0; i < numberOfDevices(); i++)
	{
		pressed = (m_devices[i]->buttonPressed(button) || pressed);
	}

	return pressed;
}

//=============================================================================
// Function: float xAxis()
// Description:
// Checks the current status of the x axis.
// Output:
// Returns between -1 and 1 if there's axis movement.
// Returns 0 if there's no axis movement.
//=============================================================================
float InputComponent::xAxis()
{
	float xAmount = 0.0f;

	for(unsigned int i = 0; i < m_devices.size(); i++)
	{
		if (m_devices[i])
		{
			switch (m_devices[i]->type())
			{
			case InputDevice::DEVICE_TYPE::GAMEPAD:
			{
				Controller *controller = static_cast<Controller*>(m_devices[i]);

				if (controller->xAxis() != 0)
				{
					xAmount = controller->xAxis();
				}

				break;
			}
			case InputDevice::DEVICE_TYPE::KEYBOARD:
			{
				Keyboard *keyboard = static_cast<Keyboard*>(m_devices[i]);

				// TODO: Make your own mapping system for the keyboard.
				if (keyboard->buttonPressed(SDLK_a))
				{
					xAmount = -1.0f;
				}
				else if (keyboard->buttonPressed(SDLK_d))
				{
					xAmount = 1.0f;
				}
			}
			}
		}
	}

	return xAmount;
}

//=============================================================================
// Function: float yAxis()
// Description:
// Gets the direction of the Y axis.
// Parameters:
// Output:
// Outputs a number between -1 and 1.
//=============================================================================
float InputComponent::yAxis()
{
	float yAmount = 0.0f;

	for (unsigned int i = 0; i < m_devices.size(); i++)
	{
		if (m_devices[i])
		{
			switch (m_devices[i]->type())
			{
			case InputDevice::DEVICE_TYPE::GAMEPAD:
			{
				Controller *controller = static_cast<Controller*>(m_devices[i]);

				if (controller->yAxis() != 0)
				{
					yAmount = controller->yAxis();
				}

				break;
			}
			case InputDevice::DEVICE_TYPE::KEYBOARD:
			{
				Keyboard *keyboard = static_cast<Keyboard*>(m_devices[i]);

				if (keyboard->buttonPressed(SDLK_w))
				{
					yAmount = -1.0f;
				}
				else if (keyboard->buttonPressed(SDLK_s))
				{
					yAmount = 1.0f;
				}
			}
			}
		}
	}

	return yAmount;
}

//=============================================================================
// Function: float axisAngle()
// Description:
// Gets the angle of the axis' from their origin using y+ as the origin.
// Output:
// float - Returns the angle of the axis in degrees.
//=============================================================================
float InputComponent::axisAngle()
{
	float angle = 0.0f;

	float radians = (float)atan2(-xAxis(), yAxis());

	angle = radiansToDegrees(radians);

	return angle;
}

//=============================================================================
// Function: void freeDevices()
// Description:
// Frees all of the devices assigned to the component allowing them to be
// reassigned.
//=============================================================================
void InputComponent::freeDevices()
{
	for(int i = 0; i < numberOfDevices(); i++)
	{
		if (m_devices[i])
		{
			m_devices[i]->free();
		}
	}

	m_devices.resize(0);
}