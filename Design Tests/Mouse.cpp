#include "Mouse.h"
#include <iostream>

using namespace Input;

Mouse::Mouse()
	:m_position(0,0)
{
}


Mouse::~Mouse()
{
}

void Mouse::pressButton(Button button)
{
	m_buttons[button] = true;
}

void Mouse::releaseButton(Button button)
{
	m_buttons[button] = false;
}

bool Mouse::buttonPressed(Button button)
{
	auto mit = m_buttons.find(button);

	if(mit == m_buttons.end())
	{
		return false;
	}

	return mit->second;
}