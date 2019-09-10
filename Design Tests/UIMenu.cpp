#include "UIMenu.h"
#include "RenderSystem.h"
#include "UIGraphic.h"
#include "EntitySystem.h"
#include "CameraMoveMessage.h"

UIMenu::UIMenu(Vector2D position, int width, int height, string texturePath)
	:m_rect(NULL),
	m_active(true),
	m_visible(true),
	m_backgroundID(-1)
{
	m_rect = new Rectangle(position.getX(), position.getY(), width, height);

	if(texturePath != "")
	{
		int backgroundID = EntitySystem::instance()->createEntity();

		if (backgroundID != -1)
		{
			Vector2D backgroundPos(width / 2, height / 2);

			UIGraphic *background = new UIGraphic(backgroundID, backgroundPos, texturePath);

			background->setRect(width, height);

			RenderSystem::instance()->setSpriteLayer(backgroundID, RenderSystem::RENDER_UI_BACKGROUND);

			this->addItem(backgroundID, background);

			m_backgroundID = backgroundID;
		}
	}
}

UIMenu::~UIMenu()
{
	cleanUp();
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the position of the menu and all of the menu objects.
// Parameters:
// Vector2D position - The new position for the menu.
//=============================================================================
void UIMenu::setPosition(Vector2D position)
{
	Camera2D *camera = RenderSystem::instance()->camera();

	camera->getWidth();

	auto mit = m_elements.begin();

	while(mit != m_elements.end())
	{
		if(mit->second)
		{
			Vector2D elementPosition = mit->second->getPosition() - m_rect->center();

			mit->second->setPosition(elementPosition + position);
		}

		mit++;
	}

	m_rect->setCenter(position.getX(), position.getY());
}

//=============================================================================
// Function: Vector2D getPosition()
// Description:
// Gets the center position of the window.
// Output:
// Vector2D - Returns the position of the menu.
// On failure it returns a vector filled with -1, -1.
//=============================================================================
Vector2D UIMenu::getPosition()
{
	Vector2D position(-1.0f, -1.0f);

	if(m_rect)
	{
		position = m_rect->center();
	}

	return position;
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the menu. Adjusts the background size, too.
// Parameters:
// int width - The new width to set the menu to.
//=============================================================================
void UIMenu::setWidth(int width)
{
	if(m_rect)
	{
		if (0 < width) 
		{ 
			m_rect->setWidth(width); 

			UIComponent *background = getItem(m_backgroundID);

			if (background)
			{
				if (background->getType() == UIComponent::UI_GRAPHIC)
				{
					UIGraphic *g_background = static_cast<UIGraphic*>(background);

					g_background->setRect(m_rect->width(), m_rect->height());
				}
			}
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the menu. Adjusts the background size, too.
// Parameters:
// int height - The new height of the menu.
//=============================================================================
void UIMenu::setHeight(int height)
{
	if(m_rect)
	{
		if(0 < height)
		{
			m_rect->setHeight(height);

			UIComponent *background = getItem(m_backgroundID);

			if(background)
			{
				if(background->getType() == UIComponent::UI_GRAPHIC)
				{
					UIGraphic *g_background = static_cast<UIGraphic*>(background);

					g_background->setRect(m_rect->width(), m_rect->height());
				}
			}
		}
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state of the menu and its elements.
// Parameters:
// bool active - The active state to set.
//=============================================================================
void UIMenu::setActive(bool active)
{
	auto mit = m_elements.begin();

	while(mit != m_elements.end())
	{
		if(mit->second)
		{
			mit->second->setActive(active);
		}

		mit++;
	}

	m_active = active;
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visibility of the menu and elements.
// Parameters:
// bool visible - The visible state to set.
//=============================================================================
void UIMenu::setVisible(bool visible)
{
	auto mit = m_elements.begin();

	while(mit != m_elements.end())
	{
		if(mit->second)
		{
			mit->second->setVisible(visible);
		}

		mit++;
	}

	m_visible = visible;
}

//=============================================================================
// Function: int getWidth()
// Description:
// Gets the width of the menu.
// Output:
// int - Returns the width of the menu on success.
// Returns -1 on failure.
//=============================================================================
int UIMenu::getWidth()
{
	int width = -1;

	if(m_rect)
	{
		width = m_rect->width();
	}

	return width;
}

//=============================================================================
// Function: int getHeight()
// Description:
// Gets the height of the menu.
// Output:
// int - Returns the height of the menu on success.
// Returns -1 on failure.
//=============================================================================
int UIMenu::getHeight()
{
	int height = -1;

	if(m_rect)
	{
		height = m_rect->height();
	}

	return height;
}

//=============================================================================
// Function: void update(float)
// Description:
// Updates the menu and all of the elements.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void UIMenu::update(float delta)
{
	auto mit = m_elements.begin();

	while(mit != m_elements.end())
	{
		mit->second->update(delta);

		mit++;
	}
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes messages that are passed to it.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIMenu::processMessage(IMessage *message)
{
	if(message)
	{
		switch(message->type())
		{
		case IMessage::CAMERA_MOVE:
		{
			CameraMoveMessage *camera = static_cast<CameraMoveMessage*>(message);

			Vector2D menuDifference = m_rect->center() - camera->m_oldPosition;

			setPosition(camera->m_newPosition + menuDifference);

			break;
		}
		}

		auto mit = m_elements.begin();

		while(mit != m_elements.end())
		{
			mit->second->processMessage(message);

			mit++;
		}
	}
}

//=============================================================================
// Function: void addItem(int, UIComponent*)
// Description:
// Adds a UI item to the menu and ties it to its ID.
// Parameters:
// int itemID - The ID of the item to add.
// UIComponent *item - Adds an item.
//=============================================================================
void UIMenu::addItem(int itemID, UIComponent *item)
{
	if(item)
	{
		auto mit = m_elements.find(itemID);

		if(mit != m_elements.end())
		{
			delete mit->second;

			mit->second = item;
		}
		else
		{
			m_elements.insert(std::make_pair(itemID, item));
		}

		item->setPosition(m_rect->getTopLeft() + item->getPosition());
		item->setDocked(false);
		item->setActive(m_active);
		item->setVisible(m_visible);
	}
}

//=============================================================================
// Function: void removeItem(int)
// Description:
// Removes the item related to the specified ID.
// Parameters:
// int itemID - The ID of the item to remove.
//=============================================================================
void UIMenu::removeItem(int itemID)
{
	auto mit = m_elements.find(itemID);

	if (mit != m_elements.end())
	{
		delete mit->second;
		mit->second = NULL;

		mit = m_elements.erase(mit);
	}
}

//=============================================================================
// Function: UIComponent* getItem(int)
// Description:
// Gets the UI component with the provided ID.
// Parameters:
// int itemID - The ID of the UI component to get.
// Output:
// UIComponent* - Returns the found UI component on success.
// Returns NULL on failure.
//=============================================================================
UIComponent* UIMenu::getItem(int itemID)
{
	UIComponent *item = NULL;

	auto mit = m_elements.find(itemID);

	if (mit != m_elements.end())
	{
		item = mit->second;
	}

	return item;
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up all of the UI items related to the menu.
//=============================================================================
void UIMenu::cleanUp()
{
	if(m_rect)
	{
		delete m_rect;
		m_rect = NULL;
	}

	auto mit = m_elements.begin();

	while(mit != m_elements.end())
	{
		delete mit->second;
		mit->second = NULL;

		mit = m_elements.erase(mit);

		if(mit != m_elements.end())
		{
			mit++;
		}
	}
}