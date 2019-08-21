#include "InputSystem.h"
#include <iostream>
#include "Controller.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "InputMessage.h"
#include "MessageSystem.h"

using namespace Input;

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
		closeDevice(e.cdevice.which + m_DEVICE_INDEX);
		break;
	}
	case SDL_JOYDEVICEADDED:
	{
		std::cout << "Joy: " << e.jdevice.which << " added!\n";
		break;
	}
	case SDL_JOYDEVICEREMOVED:
	{
		std::cout << "Joy: " << e.jdevice.which << " removed!\n";
		break;
	}
	case SDL_CONTROLLERDEVICEADDED:
	{
		std::cout << "Device added!\n";

		int ID = e.cdevice.which;
		// TODO: Figure out if this code actually works for hot plugging.
		void addController(int controllerID);

		break;
	}
	case SDL_CONTROLLERBUTTONDOWN:
	{
		std::cout << "Device button pressed!\n";

		if (deviceExists(e.cbutton.which + m_DEVICE_INDEX))
		{
			InputDevice *device = getDevice(e.cbutton.which + m_DEVICE_INDEX);

			device->pressButton(e.cbutton.button);

			InputButtonMessage *button =
				new InputButtonMessage(e.cbutton.which + m_DEVICE_INDEX,
					device->type(), 
					(Uint32)e.cbutton.button, 
					true);

			MessageSystem::instance()->pushMessage(button);
		}

		break;
	}
	case SDL_CONTROLLERBUTTONUP:
	{
		if (deviceExists(e.cbutton.which + m_DEVICE_INDEX))
		{
			InputDevice *device = getDevice(e.cbutton.which + m_DEVICE_INDEX);

			device->releaseButton(e.cbutton.button);

			InputButtonMessage *button =
				new InputButtonMessage(e.cbutton.which + m_DEVICE_INDEX,
					device->type(), 
					(Uint32)e.cbutton.button,
					false);

			MessageSystem::instance()->pushMessage(button);
		}

		break;
	}
	case SDL_CONTROLLERAXISMOTION:
	{
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		{
			if (deviceExists(e.caxis.which + m_DEVICE_INDEX))
			{
				InputDevice *device = getDevice(e.caxis.which + m_DEVICE_INDEX);

				if (device->type() == InputDevice::GAMEPAD)
				{
					Controller *controller = static_cast<Controller*>(device);
					controller->moveLeftAxisX(e.caxis.value);

					InputAxisMessage *axis =
						new InputAxisMessage(e.caxis.which + m_DEVICE_INDEX,
							controller->type(), 
							e.caxis.axis, 
							controller->xLeftAxis());

					MessageSystem::instance()->pushMessage(axis);
				}
			}
		}
		else if(e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		{
			if (deviceExists(e.caxis.which + m_DEVICE_INDEX))
			{
				InputDevice *device = getDevice(e.caxis.which + m_DEVICE_INDEX);

				if (device->type() == InputDevice::GAMEPAD)
				{
					Controller *controller = static_cast<Controller*>(device);
					controller->moveRightAxisX(e.caxis.value);

					InputAxisMessage *axis =
						new InputAxisMessage(e.caxis.which + m_DEVICE_INDEX,
							controller->type(),
							e.caxis.axis,
							controller->xRightAxis());

					MessageSystem::instance()->pushMessage(axis);
				}
			}
		}


		if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		{
			if (deviceExists(e.caxis.which + m_DEVICE_INDEX))
			{
				InputDevice *device = getDevice(e.caxis.which + m_DEVICE_INDEX);

				if (device->type() == InputDevice::GAMEPAD)
				{
					Controller *controller = static_cast<Controller*>(device);
					controller->moveLeftAxisY(e.caxis.value);

					InputAxisMessage *axis =
						new InputAxisMessage(e.caxis.which + m_DEVICE_INDEX,
							controller->type(),
							e.caxis.axis,
							controller->yLeftAxis());

					MessageSystem::instance()->pushMessage(axis);
				}
			}
		}
		else if(e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			if (deviceExists(e.caxis.which + m_DEVICE_INDEX))
			{
				InputDevice *device = getDevice(e.caxis.which + m_DEVICE_INDEX);
				
				if (device->type() == InputDevice::GAMEPAD)
				{
					Controller *controller = static_cast<Controller*>(device);
					controller->moveRightAxisY(e.caxis.value);

					InputAxisMessage *axis =
						new InputAxisMessage(e.caxis.which + m_DEVICE_INDEX,
							controller->type(),
							e.caxis.axis,
							controller->yRightAxis());

					MessageSystem::instance()->pushMessage(axis);
				}
			}
		}

		if(e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		{
			if (deviceExists(e.caxis.which + m_DEVICE_INDEX))
			{
				InputDevice *device = getDevice(e.caxis.which + m_DEVICE_INDEX);
			
				if (device->type() == InputDevice::GAMEPAD)
				{
					Controller *controller = static_cast<Controller*>(device);
					controller->moveLeftTriggerAxis(e.caxis.value);

					InputAxisMessage *axis =
						new InputAxisMessage(e.caxis.which + m_DEVICE_INDEX,
							controller->type(),
							e.caxis.axis,
							controller->leftTriggerAxis());

					MessageSystem::instance()->pushMessage(axis);
				}
			}
		}
		else if(e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		{
			if (deviceExists(e.caxis.which + m_DEVICE_INDEX))
			{
				InputDevice *device = getDevice(e.caxis.which + m_DEVICE_INDEX);
				
				if (device->type() == InputDevice::GAMEPAD)
				{
					Controller *controller = static_cast<Controller*>(device);
					controller->moveRightTriggerAxis(e.caxis.value);

					InputAxisMessage *axis =
						new InputAxisMessage(e.caxis.which + m_DEVICE_INDEX,
							controller->type(),
							e.caxis.axis,
							controller->rightTriggerAxis());

					MessageSystem::instance()->pushMessage(axis);
				}
			}
		}

		break;
	}
	case SDL_KEYDOWN:
	{
		if (deviceExists(m_KEYBOARD_ID))
		{
			InputDevice *device = getDevice(m_KEYBOARD_ID);

			if (device)
			{
				device->pressButton(e.key.keysym.sym);

				InputButtonMessage *button =
					new InputButtonMessage(m_KEYBOARD_ID,
						device->type(),
						e.key.keysym.sym,
						true);

				MessageSystem::instance()->pushMessage(button);
			}
		}
		break;
	}
	case SDL_KEYUP:
	{
		if(deviceExists(m_KEYBOARD_ID))
		{
			InputDevice *device = getDevice(m_KEYBOARD_ID);

			if (device)
			{
				device->releaseButton(e.key.keysym.sym);

				InputButtonMessage *button =
					new InputButtonMessage(m_KEYBOARD_ID,
						device->type(),
						e.key.keysym.sym,
						false);

				MessageSystem::instance()->pushMessage(button);
			}
		}
		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		if(deviceExists(m_MOUSE_ID))
		{
			InputDevice *device = getDevice(m_MOUSE_ID);
			
			if (device)
			{
				device->pressButton(e.button.button);

				InputButtonMessage *button =
					new InputButtonMessage(m_MOUSE_ID,
						device->type(),
						e.button.button,
						true);

				MessageSystem::instance()->pushMessage(button);
			}
		}
		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		if (deviceExists(m_MOUSE_ID))
		{
			InputDevice *device = getDevice(m_MOUSE_ID);

			if (device)
			{
				device->releaseButton(e.button.button);

				InputButtonMessage *button =
					new InputButtonMessage(m_MOUSE_ID,
						device->type(),
						e.button.button,
						false);

				MessageSystem::instance()->pushMessage(button);
			}
		}

		break;
	}
	case SDL_MOUSEMOTION:
	{
		if(deviceExists(m_MOUSE_ID))
		{
			InputDevice *device = getDevice(m_MOUSE_ID);
		
			if (device)
			{
				if (device->type() == InputDevice::MOUSE)
				{
					Mouse *mouse = static_cast<Mouse*>(device);

					mouse->setX(e.motion.x);
					mouse->setY(e.motion.y);

					InputMoveMessage *move =
						new InputMoveMessage(m_MOUSE_ID,
							device->type(),
							mouse->getX(),
							mouse->getY());

					MessageSystem::instance()->pushMessage(move);
				}
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
		m_devices.insert(std::make_pair(deviceID, device));
	}
}

void InputSystem::closeDevice(int deviceID)
{
	auto mit = m_devices.find(deviceID);
	
	if(mit != m_devices.end())
	{
		delete mit->second;
		mit = m_devices.erase(mit);
	}
}

//=============================================================================
// Function: void initializeDevices()
// Description:
// Initializes the current devices that are recognized by the system.
// It always loads the mouse and keyboard first.
//=============================================================================
void InputSystem::initializeDevices()
{
	Mouse *mouse = new Mouse();
	Keyboard *keyboard = new Keyboard();

	addDevice(0, mouse);
	addDevice(1, keyboard);

	for(int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if(SDL_IsGameController(i))
		{
			Controller *controller = new Controller(i, true);
			addDevice(i + m_DEVICE_INDEX, controller);
		}
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