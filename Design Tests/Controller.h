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
#include <unordered_map>

namespace Input
{
	class Controller : public InputDevice
	{
	public:
		Controller(int controllerID, bool hapticEnabled);
		virtual ~Controller();

		virtual DEVICE_TYPE type() { return GAMEPAD; }

		virtual void pressButton(Button button);
		virtual void releaseButton(Button button);
		virtual bool buttonPressed(Button button);

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

		std::unordered_map<Button, bool>m_buttons;

		void open();
		void enableHaptic();
		void disableHaptic();
		void close();
	};
}

