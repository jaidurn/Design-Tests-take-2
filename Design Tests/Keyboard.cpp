#include "Keyboard.h"

using namespace Input;

Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

void Keyboard::pressKey(Uint32 key)
{
	m_keys[key] = true;
}

void Keyboard::releaseKey(Uint32 key)
{
	m_keys[key] = false;
}

bool Keyboard::keyPressed(Uint32 key)
{
	auto mit = m_keys.find(key);

	if(mit == m_keys.end())
	{
		return false;
	}

	return mit->second;
}
