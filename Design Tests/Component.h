#pragma once
//==========================================================================================
// File Name: Component.h
// Author: Brian Blackmon
// Date Created: 3/25/2019
// Purpose: 
// Component base class.
//==========================================================================================
#include <string>

class Component
{
public:
	enum componentID
	{
		NONE,
		COLLISION,
		VELOCITY,
		SPRITE,
		ANIMATION,
		INPUT,
		STATE,
		LOGIC
	};

	Component(componentID id);
	~Component();

	componentID ID() { return m_ID; }

private:
	componentID m_ID;
};

