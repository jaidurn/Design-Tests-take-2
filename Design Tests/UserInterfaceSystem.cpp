#include "UserInterfaceSystem.h"
#include "UIButton.h"

UserInterfaceSystem::~UserInterfaceSystem()
{
	cleanUp();
}

//=============================================================================
// Function: UIComponent *createUI(UI_TYPE, int, Vector2D)
// Description:
// Creates a UI component with default values.
// Parameters:
// UI_TYPE type - The type of UI to create.
// int entityID - The ID of the UI to create.
// Vector2D position - The point to create the UI at.
// Output:
// UIComponent* - Returns a UI component object on successful creation.
// Returns NULL on failure.
//=============================================================================
UIComponent* UserInterfaceSystem::createUI(UIComponent::UI_TYPE type, int entityID, Vector2D position)
{
	UIComponent *comp = NULL;

	auto mit = m_UIcomponents.find(entityID);

	if (mit != m_UIcomponents.end())
	{
		comp = mit->second;
	}
	else 
	{
		switch(type)
		{
		case UIComponent::UI_BUTTON:
		{
			comp = new UIButton(entityID, position, NULL, "");
			break;
		}
		case UIComponent::UI_TEXT:
		{
			break;
		}
		case UIComponent::UI_GRAPHIC:
		{
			break;
		}
		}

		if(comp)
		{
			m_UIcomponents.insert(std::make_pair(entityID, comp));
		}
	}

	return comp;
}

//=============================================================================
// Function: UIComponent* getUI(int)
// Description:
// Gets the UIComponent for the entity ID.
// Parameters:
// int entityID - The component ID.
// Output:
// UIComponent* - Returns the UI found.
// Returns NULL if the UI item isn't found.
//=============================================================================
UIComponent* UserInterfaceSystem::getUI(int entityID)
{
	UIComponent *comp = NULL;

	auto mit = m_UIcomponents.find(entityID);

	if(mit != m_UIcomponents.end())
	{
		comp = mit->second;
	}

	return comp;
}

//=============================================================================
// Function: void update(float)
// Description:
// Updates all of the UI components.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void UserInterfaceSystem::update(float delta)
{
	auto mit = m_UIcomponents.begin();

	while(mit != m_UIcomponents.end())
	{
		mit->second->update(delta);
		mit++;
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Deletes all of the existing UI components, frees memory, and
// empties the component map.
//=============================================================================
void UserInterfaceSystem::cleanUp()
{
	auto mit = m_UIcomponents.begin();

	while(mit != m_UIcomponents.end())
	{
		delete mit->second;
		mit->second = NULL;

		mit = m_UIcomponents.erase(mit);

		if(mit != m_UIcomponents.end())
		{
			mit++;
		}
	}
}