#pragma once
//==========================================================================================
// File Name: InputComponent.h
// Author: Brian Blackmon
// Date Created: 4/22/2019
// Purpose: 
// Holds the input information for an entity.
//==========================================================================================
#include "Component.h"
#include "InputDevice.h"
#include <vector>

namespace Input
{
	class InputComponent : public Component
	{
	public:
		InputComponent();
		~InputComponent();

		int numberOfDevices();

		InputDevice *getDevice(int deviceNumber);
		InputDevice::DEVICE_TYPE getDeviceType(int deviceNumber);

		void addDevice(InputDevice *device);

		bool buttonPressed(Button button);

		float xAxis();
		float yAxis();
		float axisAngle();

	private:
		std::vector<InputDevice*> m_devices;

		void freeDevices();
	};
}

