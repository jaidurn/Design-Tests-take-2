#pragma once
//==========================================================================================
// File Name: KeyboardMouse.h
// Author: Brian Blackmon
// Date Created: 9/20/2019
// Purpose: 
// Makes the keyboard and mouse a uniform game controller.
//==========================================================================================
#include "InputDevice.h"
#include "Vector2D.h"

class KeyboardMouse : public InputDevice
{
public:
	KeyboardMouse(int deviceID);
	virtual ~KeyboardMouse();

	virtual void mapButton(BUTTON_CODE buttonCode, Button button);

	virtual void pressButton(Button button);
	virtual void releaseButton(Button button);

	virtual bool buttonPressed(BUTTON_CODE buttonCode);

	Vector2D getPosition();
	void setPosition(Vector2D position);
	void setPosition(int x, int y);

protected:
	virtual void initialize();

	virtual void sendInputButtonMsg(BUTTON_CODE buttonCode, bool pressed);
	void sendInputMoveMsg();

private:
	Vector2D m_position;
};
