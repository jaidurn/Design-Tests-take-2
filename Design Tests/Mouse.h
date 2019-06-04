#pragma once
//==========================================================================================
// File Name: Mouse.h
// Author: Brian Blackmon
// Date Created: 4/29/2019
// Purpose: 
// Holds the information for the mouse.
//==========================================================================================
#include "InputDevice.h"
#include "Vector2D.h"
#include <SDL.h>
#include <unordered_map>

namespace Input
{
	class Mouse : public InputDevice
	{
	public:
		Mouse();
		~Mouse();

		virtual DEVICE_TYPE type() { return MOUSE; }

		virtual void pressButton(Button button);
		virtual void releaseButton(Button button);
		virtual bool buttonPressed(Button button);

		void setX(float x) { m_position.setX(x); }
		void setX(int x) { m_position.setX((float)(x)); }
		void setY(float y) { m_position.setY(y); }
		void setY(int y) { m_position.setY((float)(y)); }

	private:
		Vector2D m_position;
		std::unordered_map<Button, bool>m_buttons;
	};
}