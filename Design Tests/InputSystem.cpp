#include "InputSystem.h"
#include <iostream>
#include "Controller.h"
#include "KeyboardMouse.h"
#include "InputMessage.h"
#include "MessageSystem.h"

InputSystem::InputSystem()
	:m_exit(false)
{
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);
	SDL_GameControllerEventState(SDL_ENABLE);
	initializeDevices();
}


InputSystem::~InputSystem()
{
	auto mit = m_devices.begin();

	while(mit != m_devices.end())
	{
		delete mit->second;
		mit = m_devices.erase(mit);

		if(mit != m_devices.end())
		{
			mit++;
		}
	}

	auto cmit = m_components.begin();

	while(cmit != m_components.end())
	{
		delete cmit->second;

		cmit = m_components.erase(cmit);

		if (cmit != m_components.end())
		{
			cmit++;
		}
	}
}

void InputSystem::processInput(SDL_Event &e)
{
	switch (e.type)
	{
	case SDL_CONTROLLERDEVICEREMOVED:
	{
		std::cout << "Controller removed: " << e.cdevice.which << std::endl;
		
		closeDevice(e.cdevice.which);

		break;
	}
	case SDL_CONTROLLERDEVICEADDED:
	{
		std::cout << "Controller: " << e.jdevice.which << " added!\n";

		int deviceID = (int)m_devices.size();

		Controller *controller = new Controller(deviceID, e.jdevice.which, true);

		addDevice(deviceID, controller);

		std::cout << "Device: " << deviceID << " added!\n";

		break;
	}
	case SDL_CONTROLLERBUTTONDOWN:
	{
		Controller *controller = getController(e.cbutton.which);

		if (controller)
		{
			controller->pressButton(e.cbutton.button);
		}

		break;
	}
	case SDL_CONTROLLERBUTTONUP:
	{
		Controller *controller = getController(e.cbutton.which);

		if (controller)
		{
			controller->releaseButton(e.cbutton.button);
		}

		break;
	}
	case SDL_CONTROLLERAXISMOTION:
	{
		Controller *controller = getController(e.caxis.which);

		if (controller)
		{
			std::cout << "Controller: " << e.caxis.which << " axis moved!\n";

			if(e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
			{
				controller->moveLeftAxisX(e.caxis.value);
			}
			else if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
			{
				controller->moveRightAxisX(e.caxis.value);
			}

			if(e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
			{
				controller->moveLeftAxisY(e.caxis.value);
			}
			else if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
			{
				controller->moveRightAxisY(e.caxis.value);
			}

			if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
			{
				controller->moveLeftTriggerAxis(e.caxis.value);
			}
			else if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
			{
				controller->moveRightTriggerAxis(e.caxis.value);
			}
		}

		break;
	}
	case SDL_KEYDOWN:
	{
		if (deviceExists(m_KEYBOARDMOUSE_INDEX))
		{
			InputDevice *device = m_devices[m_KEYBOARDMOUSE_INDEX];

			KeyboardMouse *kbm = static_cast<KeyboardMouse*>(device);

			kbm->pressButton(e.key.keysym.sym);
		}

		break;
	}
	case SDL_KEYUP:
	{
		if (deviceExists(m_KEYBOARDMOUSE_INDEX))
		{
			InputDevice *device = m_devices[m_KEYBOARDMOUSE_INDEX];

			KeyboardMouse *kbm = static_cast<KeyboardMouse*>(device);

			kbm->releaseButton(e.key.keysym.sym);
		}

		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		if(deviceExists(m_KEYBOARDMOUSE_INDEX))
		{
			InputDevice *device = getDevice(m_KEYBOARDMOUSE_INDEX);
			
			device->pressButton(e.button.button);
		}

		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		if (deviceExists(m_KEYBOARDMOUSE_INDEX))
		{
			InputDevice *device = getDevice(m_KEYBOARDMOUSE_INDEX);

			device->releaseButton(e.button.button);
		}

		break;
	}
	case SDL_MOUSEMOTION:
	{
		if (deviceExists(m_KEYBOARDMOUSE_INDEX))
		{
			InputDevice *device = getDevice(m_KEYBOARDMOUSE_INDEX);

			if (device->getDeviceType() == InputDevice::KEYBOARD_MOUSE)
			{
				KeyboardMouse *kbm =
					static_cast<KeyboardMouse*>(device);

				kbm->setPosition(e.motion.x, e.motion.y);
			}
		}

		break;
	}
	case SDL_QUIT:
	{
		m_exit = true;
		break;
	}
	}
}

//=============================================================================
// Function: InputComponent* createInputComponent(int)
// Description:
// Creates an input component and adds it to the multimap.
// Use this function to add multiple controllers to the same character.
// Only make multiple controllers bind to the same controller if you know
// what you're doing. This is mostly for debug purposes.
// Parameters:
// int ID - The entity ID to create an input component for.
// Output:
// InputComponent* - The input component that was created.
//=============================================================================
InputComponent* InputSystem::createInputComponent(int ID)
{
	auto mit = m_components.find(ID);

	InputComponent *input = NULL;

	if(mit != m_components.end())
	{
		input = mit->second;
	}
	else
	{
		input = new InputComponent();

		m_components.insert(std::make_pair(ID, input));
	}

	return input;
}

InputComponent* InputSystem::getInputComponent(int ID)
{
	InputComponent *input = NULL;

	auto mit = m_components.find(ID);

	if(mit != m_components.end())
	{
		input = mit->second;
	}

	return input;
}

//=============================================================================
// Function: InputDevice* getDevice(int)
// Description:
// Requests the device attached to the ID.
// Parameters:
// int deviceID - The ID of the device.
//=============================================================================
InputDevice* InputSystem::getDevice(int ID)
{
	auto mit = m_devices.find(ID);

	if(mit == m_devices.end())
	{
		return NULL;
	}

	return mit->second;
}

//=============================================================================
// Function: Controller* getController(int)
// Description:
// Finds the controller with the specified instanceID.
// Parameters:
// int instanceID - The instance ID of the controller to find.
// Output:
// Controller*
// On success returns a pointer to the found controller object.
// On failure returns NULL.
//=============================================================================
Controller* InputSystem::getController(int instanceID)
{
	Controller* controller = NULL;

	auto mit = m_devices.begin();

	while (mit != m_devices.end())
	{
		if (mit->second->getDeviceType() == InputDevice::GAMEPAD)
		{
			Controller *check = static_cast<Controller*>(mit->second);

			if (check->getInstanceID() == instanceID)
			{
				controller = check;

				break;
			}
		}

		mit++;
	}

	return controller;
}

//=============================================================================
// Function: InputDevice* getNextFreeDevice()
// Description:
// Finds the next device that hasn't been assigned and assigns it.
// Output:
// InputDevice* - The free device.
//=============================================================================
InputDevice* InputSystem::getNextFreeDevice()
{
	InputDevice *device = NULL;

	auto mit = m_devices.begin();

	while(device == NULL && mit != m_devices.end())
	{
		if(!mit->second->isAssigned())
		{
			device = mit->second;
			device->assign();
		}

		mit++;
	}

	return device;
}

//=============================================================================
// Function: void initializeDevices()
// Description:
// Initializes the current devices that are recognized by the system.
// It always loads the mouse and keyboard first.
//=============================================================================
void InputSystem::initializeDevices()
{
	KeyboardMouse *kbm = new KeyboardMouse(m_KEYBOARDMOUSE_INDEX);

	addDevice(m_KEYBOARDMOUSE_INDEX, kbm);
}

//=============================================================================
// Function: void addDevice(int, InputDevice*)
// Description: 
// Adds a device to the list of current devices.
// Parameters:
// int deviceID - The ID of the device.
// InputDevice* device - The device to add.
//=============================================================================
void InputSystem::addDevice(int deviceID, InputDevice *device)
{
	auto mit = m_devices.find(deviceID);

	if(mit == m_devices.end())
	{
		std::cout << "Device: " << deviceID << " added!\n";

		m_devices.insert(std::make_pair(deviceID, device));
	}
	else
	{
		std::cout << "Failed to add device with id " << deviceID  << "!\n";

		delete device;
		device = NULL;
	}
}

void InputSystem::closeDevice(int instanceID)
{
	auto mit = m_devices.begin();

	while (mit != m_devices.end())
	{
		if (mit->second->getDeviceType() == InputDevice::GAMEPAD)
		{
			Controller *controller = static_cast<Controller*>(mit->second);

			if (controller->getInstanceID() == instanceID)
			{
				delete mit->second;

				std::cout << "Device: " << mit->first << " closed!\n";

				mit = m_devices.erase(mit);

				break;
			}
		}

		mit++;
	}
}

bool InputSystem::deviceExists(int deviceID)
{
	auto mit = m_devices.find(deviceID);

	if(mit == m_devices.end())
	{
		return false;
	}

	if(!mit->second)
	{
		return false;
	}

	return true;
}