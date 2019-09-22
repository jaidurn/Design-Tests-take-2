#pragma once
//==========================================================================================
// File Name: InputMessage.h
// Author: Brian Blackmon
// Date Created: 8/15/2019
// Purpose: 
// Holds information about an input event that happened.
//==========================================================================================
#include "IMessage.h"
#include "InputDevice.h"
#include <SDL.h>

class InputMessage : public IMessage
{
public:
	enum InputType
	{
		INPUT_BUTTON,
		INPUT_AXIS,
		INPUT_MOVE
	};

	InputMessage(int deviceID, InputDevice::DEVICE_TYPE deviceType, InputType inputType)
		:IMessage(INPUT), 
		m_deviceID(deviceID), 
		m_deviceType(deviceType), 
		m_inputType(inputType)
	{

	}

	virtual ~InputMessage() {}

	InputDevice::DEVICE_TYPE m_deviceType;
	InputType m_inputType;
	int m_deviceID;
};

class InputButtonMessage : public InputMessage
{
public:
	InputButtonMessage(int deviceID, InputDevice::DEVICE_TYPE deviceType, InputDevice::BUTTON_CODE buttonCode, bool pressed)
		:InputMessage(deviceID, deviceType, INPUT_BUTTON), 
		m_pressed(pressed),
		m_buttonCode(buttonCode)
	{
	}

	virtual ~InputButtonMessage() {}

	InputDevice::BUTTON_CODE m_buttonCode;
	bool m_pressed;
};

class InputAxisMessage : public InputMessage
{
public:
	InputAxisMessage(int deviceID, InputDevice::DEVICE_TYPE deviceType, Uint8 axis, float axisValue)
		:InputMessage(deviceID, deviceType, INPUT_AXIS),
		m_axis(axis),
		m_axisMovement(axisValue)
	{

	}

	virtual ~InputAxisMessage() {}

	Uint8 m_axis;
	float m_axisMovement;
};

class InputMoveMessage : public InputMessage
{
public:
	InputMoveMessage(int deviceID, InputDevice::DEVICE_TYPE deviceType, float x, float y)
		:InputMessage(deviceID, deviceType, INPUT_MOVE),
		m_x(x),
		m_y(y)
	{
	}

	virtual ~InputMoveMessage() {}

	float m_x;
	float m_y;
};