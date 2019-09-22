#pragma once
//==========================================================================================
// File Name: InputSystem.h
// Author: Brian Blackmon
// Date Created: 4/22/2019
// Purpose: 
// Handles input components and input event handling.
//==========================================================================================
#include <map>
#include <SDL.h>
#include <unordered_map>
#include <map>
#include "InputDevice.h"
#include "InputComponent.h"

class Controller;

class InputSystem
{
public:
	static InputSystem* instance()
	{
		static InputSystem *instance = new InputSystem();

		return instance;
	}

	~InputSystem();

	void processInput(SDL_Event &e);

	bool exit() { return m_exit; }

	InputComponent* createInputComponent(int ID);
	InputComponent* getInputComponent(int ID);

	InputDevice* getDevice(int ID);
	Controller* getController(int instanceID);
	InputDevice* getNextFreeDevice();

private:
	InputSystem();

	bool m_exit;
	const int m_DEVICE_INDEX = 1;
	const int m_KEYBOARDMOUSE_INDEX = 0;

	std::unordered_map<int, InputDevice*>m_devices;
	std::map<int, InputComponent*>m_components;

	void initializeDevices();
	void addDevice(int deviceID, InputDevice *device);
	void closeDevice(int instanceID);
	bool deviceExists(int deviceID);	
};


