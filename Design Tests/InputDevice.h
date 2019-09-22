#pragma once
//==========================================================================================
// File Name: InputDevice.h
// Author: Brian Blackmon
// Date Created: 4/29/2019
// Purpose: 
// A virtual base class for all input devices.
//==========================================================================================
#include <SDL.h>
#include <map>
#include "MessageSystem.h"

class InputDevice
{
public:
	typedef Uint32 Button;

	enum DEVICE_TYPE
	{
		NONE = -1,
		MOUSE = 0,
		KEYBOARD = 1,
		GAMEPAD = 2,
		KEYBOARD_MOUSE
	};

	enum BUTTON_CODE
	{
		ANALOG_UP,
		ANALOG_RIGHT,
		ANALOG_DOWN,
		ANALOG_LEFT,
		SELECT,
		BACK,
		ATTACK_1,
		ATTACK_2,
		SPECIAL_1,
		SPECIAL_2,
		TOTAL_BUTTONS
	};

	InputDevice(int deviceID, DEVICE_TYPE type)
	: m_deviceID(deviceID),
	  m_deviceType(type),
	  m_isAssigned(false)
	{

	}

	~InputDevice() {}

	virtual void mapButton(BUTTON_CODE buttonCode, Button button) = 0;

	virtual void pressButton(Button button) = 0;
	virtual void releaseButton(Button button) = 0;

	virtual bool buttonPressed(BUTTON_CODE buttonCode) = 0;

	void assign() { m_isAssigned = true; }
	void free() { m_isAssigned = false; }
	bool isAssigned() { return m_isAssigned; }

	int getID() { return m_deviceID; }
	DEVICE_TYPE getDeviceType() { return m_deviceType; }

protected:
	int m_deviceID;
	bool m_isAssigned;

	DEVICE_TYPE m_deviceType;

	std::map<Button, BUTTON_CODE> m_buttonMap;
	std::map<BUTTON_CODE, bool> m_buttonState;

	virtual void initialize()
	{
		for (unsigned int i = ANALOG_UP; i < TOTAL_BUTTONS; i++)
		{
			m_buttonState.insert(std::make_pair((BUTTON_CODE)i, false));
		}
	}

	virtual void sendInputButtonMsg(BUTTON_CODE buttonCode, bool pressed) = 0;
};

