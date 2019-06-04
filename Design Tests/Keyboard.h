#pragma once
//==========================================================================================
// File Name: Keyboard.h
// Author: Brian Blackmon
// Date Created: 4/29/2019
// Purpose: 
//  Wraps the keyboard information for SDL.
//==========================================================================================
#include "InputDevice.h"
#include <SDL.h>
#include <unordered_map>

namespace Input
{
	class Keyboard : public InputDevice
	{
	public:
		Keyboard();
		virtual ~Keyboard();

		virtual DEVICE_TYPE type() { return KEYBOARD; }

		virtual void pressButton(Button button) { pressKey(button); }
		virtual void releaseButton(Button button) { releaseKey(button); }
		virtual bool buttonPressed(Button button) { return keyPressed(button); }

	private:
		std::unordered_map<Uint32, bool> m_keys;

		void pressKey(Uint32 key);
		void releaseKey(Uint32 key);

		bool keyPressed(Uint32 key);
	};
}