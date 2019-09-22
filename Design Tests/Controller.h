#pragma once
//==========================================================================================
// File Name: Controller.h
// Author: Brian Blackmon
// Date Created: 4/22/2019
// Purpose: 
// Wraps the SDL_Controller struct and adds functionality.
//==========================================================================================
#include "InputDevice.h"
#include <SDL.h>

class Controller : public InputDevice
{
public:
	Controller(int deviceID, int controllerID, bool hapticEnabled);
	virtual ~Controller();

	virtual void mapButton(BUTTON_CODE buttonCode, Button button);

	virtual void pressButton(Button button);
	virtual void releaseButton(Button button);
	virtual bool buttonPressed(BUTTON_CODE buttonCode);

	void moveLeftAxisX(Sint16 movement);
	void moveLeftAxisY(Sint16 movement);

	void moveRightAxisX(Sint16 movement);
	void moveRightAxisY(Sint16 movement);

	void moveLeftTriggerAxis(Sint16 movement);
	void moveRightTriggerAxis(Sint16 movement);

	void triggerHaptic(float strength, Uint32 length);
	void toggleHapticOnOff();

	float xLeftAxis() { return m_xLeftAxis; }
	float yLeftAxis() { return m_yLeftAxis; }

	float xRightAxis() { return m_xRightAxis; }
	float yRightAxis() { return m_yRightAxis; }

	float leftTriggerAxis() { return m_leftTriggerAxis; }
	float rightTriggerAxis() { return m_rightTriggerAxis; }

	int getInstanceID();

protected:
	virtual void sendInputButtonMsg(BUTTON_CODE buttonCode, bool pressed);
	void sendInputAxisMsg(Uint8 axis, float amount);

	virtual void initialize();

private:
	SDL_GameController *m_controller;
	SDL_Haptic *m_haptic;
	int m_ID;

	float m_xLeftAxis;
	float m_yLeftAxis;

	float m_xRightAxis;
	float m_yRightAxis;

	float m_leftTriggerAxis;
	float m_rightTriggerAxis;

	const int m_DEAD_ZONE = 8000;
	const int m_AXIS_MAX = 32768;

	void open();
	void enableHaptic();
	void disableHaptic();
	void close();
};
