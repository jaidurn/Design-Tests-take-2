#pragma once
//==========================================================================================
// File Name: UserInterfaceSystem.h
// Author: Brian Blackmon
// Date Created: 8/15/2019
// Purpose: 
// Handles the creation, logic, and deletion of UI elements.
//==========================================================================================
#include "UIComponent.h"
#include <map>

class UserInterfaceSystem
{
public:
	static UserInterfaceSystem *instance()
	{
		static UserInterfaceSystem *instance = new UserInterfaceSystem();

		return instance;
	}

	~UserInterfaceSystem();

	UIComponent *createUI(UIComponent::UI_TYPE type, int entityID, Vector2D position);
	UIComponent *getUI(int entityID);

	void update(float delta);

private:
	UserInterfaceSystem()
	{
	}

	std::map<int, UIComponent*> m_UIcomponents;

	void cleanUp();
};

