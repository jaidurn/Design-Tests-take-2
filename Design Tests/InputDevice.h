#pragma once
//==========================================================================================
// File Name: InputDevice.h
// Author: Brian Blackmon
// Date Created: 4/29/2019
// Purpose: 
// A virtual base class for all input devices.
//==========================================================================================
#include <SDL.h>

class InputDevice
{
public:
	typedef Uint32 Button;

	enum DEVICE_TYPE
	{
		NONE = -1,
		MOUSE = 0,
		KEYBOARD = 1,
		GAMEPAD = 2
	};

	InputDevice()
	:m_isAssigned(false)
	{
	}

	~InputDevice() {}

	virtual DEVICE_TYPE type() = 0;

	virtual void pressButton(Button button) = 0;
	virtual void releaseButton(Button button) = 0;

	virtual bool buttonPressed(Button button) = 0;

	void assign() { m_isAssigned = true; }
	void free() { m_isAssigned = false; }
	bool isAssigned() { return m_isAssigned; }

protected:
	bool m_isAssigned;
};

