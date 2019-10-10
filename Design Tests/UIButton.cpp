#include "UIButton.h"
#include "UIText.h"
#include "UIGraphic.h"
#include "MessageSystem.h"
#include "EntityDestroyMessage.h"
#include "PhysicsSystem.h"
#include "InputMessage.h"
#include "InputSystem.h"
#include "KeyboardMouse.h"
#include "RenderSystem.h"

UIButton::UIButton(int entityID,
	Vector2D position,
	string name,
	bool toggle)
	:UIComponent(entityID, UI_BUTTON, position, name),
	m_textUI(NULL),
	m_background(NULL),
	m_toggle(toggle),
	m_pressed(false),
	m_wasPressed(false),
	m_selected(false)
{

}

UIButton::~UIButton()
{
	if (m_textUI)
	{
		delete m_textUI;
		m_textUI = NULL;
	}

	if (m_background)
	{
		delete m_background;
		m_background = NULL;
	}

	EntityDestroyMessage *destroy = new EntityDestroyMessage(m_entityID);

	MessageSystem::instance()->pushMessage(destroy);
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Sets the position of the button and related components.
// Parameters:
// Vector2D position - The new center position.
//=============================================================================
void UIButton::setPosition(Vector2D position)
{
	if (m_background)
	{
		m_background->setPosition(position);
	}

	if (m_rect)
	{
		m_rect->setCenter(position);
	}

	if (m_textUI)
	{
		m_textUI->setPosition(position);
	}
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the button and related components.
// Parameters:
// int width - The new width of the button.
//=============================================================================
void UIButton::setWidth(int width)
{
	if (m_rect)
	{
		if (m_background)
		{
			m_background->setWidth(width);
		}

		if (m_textUI)
		{
			int textBorder = 20;

			m_textUI->setWidth(width - textBorder);
		}

		m_rect->setWidth(width);
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the button and related components.
// Parameters:
// int height - The new height of the button.
//=============================================================================
void UIButton::setHeight(int height)
{
	if (m_rect)
	{
		if (m_background)
		{
			m_background->setHeight(height);
		}

		if (m_textUI)
		{
			int textBorder = 20;

			m_textUI->setHeight(height - textBorder);
		}

		m_rect->setHeight(height);
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visible state of the button and related components.
// Parameters:
// bool visible - The new visible state of the button.
//=============================================================================
void UIButton::setVisible(bool visible)
{
	m_visible = visible;

	if (m_background)
	{
		m_background->setVisible(m_visible);
	}

	if (m_textUI)
	{
		m_textUI->setVisible(m_visible);
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state of the button and related components.
// Parameters:
// bool active - The new active state of the button.
//=============================================================================
void UIButton::setActive(bool active)
{
	m_active = active;

	if (m_background)
	{
		m_background->setActive(active);
	}

	if (m_textUI)
	{
		m_textUI->setActive(active);
	}
}

//=============================================================================
// Function: void setSelected(bool)
// Description:
// Sets the selected state of the button.
// Parameters:
// bool selected - The selected state to set.
//=============================================================================
void UIButton::setSelected(bool selected)
{
	if (m_selected != selected)
	{
		m_selected = selected;
		updateSelected();
	}
}

//=============================================================================
// Function: void setTextOffset(Vector2D)
// Description:
// Sets the offset of the text from the center of the button.
// Parameters:
// Vector2D offset - The new text offset.
//=============================================================================
void UIButton::setTextOffset(Vector2D offset)
{
	if (m_textUI)
	{
		m_textUI->setPosition(m_rect->center() + offset);
	}
}

//=============================================================================
// Function: void setTextUI(UIText*)
// Description:
// Sets the textUI for the button.
// Parameters:
// UIText *textUI - The text UI object to use as the text.
//=============================================================================
void UIButton::setTextUI(UIText *textUI)
{
	if (textUI)
	{
		if (m_textUI)
		{
			delete m_textUI;
			m_textUI = NULL;
		}

		m_textUI = textUI;

		if (m_rect)
		{
			m_textUI->setWidth(m_rect->width());
			m_textUI->setHeight(m_rect->height());

			m_textUI->setPosition(m_rect->center());
		}
	}
}

//=============================================================================
// Function: void setText(string)
// Description:
// Sets the text on the button to the new text if there's
// an existing text ui.
// Parameters:
// string text - The new text to display on the button.
//=============================================================================
void UIButton::setText(string text)
{
	if (m_textUI)
	{
		m_textUI->setText(text);
	}
}

//=============================================================================
// Function: void setBackground(UIGraphic*)
// Description:
// Sets the background to the provided UIGraphic.
// Parameters:
// UIGraphic *background - The new background to use for the button.
//=============================================================================
void UIButton::setBackground(UIGraphic *background)
{
	if (background)
	{
		if (m_background)
		{
			delete m_background;
			m_background = NULL;
		}

		m_background = background;

		m_background->setWidth(m_rect->width());
		m_background->setHeight(m_rect->height());
		m_background->setPosition(m_rect->center());
	}
}

//=============================================================================
// Function: void setPressed(bool)
// Description:
// Sets the current pressed state to the specified state.
// Parameters:
// bool pressed - The new pressed state.
//=============================================================================
void UIButton::setPressed(bool pressed)
{
	m_wasPressed = m_pressed;
	m_pressed = pressed;
}

//=============================================================================
// Function: bool getPressed()
// Description:
// Checks if the button was pressed or not.
// Output:
// bool
// Returns true if the button was recently pressed.
// Returns false if the button isn't pressed.
//=============================================================================
bool UIButton::getPressed()
{
	bool pressed = false;
	
	if (!m_wasPressed && m_pressed)
	{
		pressed = true;
	}

	return pressed;
}

//=============================================================================
// Function: bool getReleased()
// Description:
// Checks if the button was released.
// Output:
// bool
// Returns true if the button was released.
// Returns false if the button is pressed, or has been released for
// a while.
//=============================================================================
bool UIButton::getReleased()
{
	bool released = false;

	if (m_wasPressed && !m_pressed)
	{
		released = true;
	}

	return released;
}

//=============================================================================
// Function: bool getHeld()
// Description:
// Checks if the button is being held.
// Output:
// bool
// Returns true if the button is still being pressed.
// Returns false if the button was released or just pressed.
//=============================================================================
bool UIButton::getHeld()
{
	bool held = false;

	if (m_wasPressed && m_pressed)
	{
		held = true;
	}

	return held;
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes the messages passed to the button.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIButton::processMessage(IMessage *message)
{
	if (message)
	{
		switch (message->type())
		{
		case IMessage::INPUT:
		{
			InputMessage *inputMsg = static_cast<InputMessage*>(message);

			processInput(inputMsg);

			break;
		}
		case IMessage::ENTITY_DESTROY:
		{
			EntityDestroyMessage *destroyMsg = static_cast<EntityDestroyMessage*>(message);

			processEntityDestroy(destroyMsg);

			break;
		}
		}
	}
}

//=============================================================================
// Function: void update()
// Description:
// Handles the updates for the button. Updates the state of the button.
//=============================================================================
void UIButton::update()
{
	if (m_active)
	{
		m_wasPressed = m_pressed;
	}
}

//=============================================================================
// Function: void processInput(InputMessage*)
// Description:
// Handles what to do for input.
// Parameters:
// InputMessage *inputMsg - The input message to process.
//=============================================================================
void UIButton::processInput(InputMessage *inputMsg)
{
	// If the button is active and its rect exists
	if (m_active && m_rect && inputMsg)
	{
		CollisionSystem *collisionSys = PhysicsSystem::instance()->collisionSystem();

		if (collisionSys)
		{
			switch (inputMsg->m_inputType)
			{
			case InputMessage::INPUT_BUTTON:
			{
				InputButtonMessage *button = static_cast<InputButtonMessage*>(inputMsg);

				if (button->m_deviceType == InputDevice::KEYBOARD_MOUSE)
				{
					InputDevice *inputDevice = InputSystem::instance()->getDevice(button->m_deviceID);

					KeyboardMouse *kbm = static_cast<KeyboardMouse*>(inputDevice);
					
					Vector2D mousePosition = kbm->getPosition();

					Camera2D *camera = RenderSystem::instance()->camera();

					// Scale the mouse position to the proper spot.
					if (camera)
					{
						mousePosition.setX(mousePosition.getX() + camera->getX());
						mousePosition.setY(mousePosition.getY() + camera->getY());
					}

					if (collisionSys->pointInsideRect(m_rect, (int)mousePosition.getX(), (int)mousePosition.getY()))
					{
						if (button->m_buttonCode == InputDevice::SELECT)
						{
							m_pressed = button->m_pressed;
						}
					}
				}
				else
				{
					if (button->m_buttonCode == InputDevice::SELECT)
					{
						m_pressed = button->m_pressed;
					}
				}

				break;
			}
			case InputMessage::INPUT_MOVE:
			{
				InputMoveMessage *inputMove = static_cast<InputMoveMessage*>(inputMsg);

				if (inputMove->m_deviceType == InputDevice::KEYBOARD_MOUSE)
				{
					Vector2D mousePos(inputMove->m_x, inputMove->m_y);

					Camera2D *camera = RenderSystem::instance()->camera();

					if (camera)
					{
						mousePos.setX(mousePos.getX() + (float)camera->getX());
						mousePos.setY(mousePos.getY() + (float)camera->getY());
					}

					if (!collisionSys->pointInsideRect(m_rect, (int)mousePos.getX(), (int)mousePos.getY()))
					{
						m_pressed = false;
						m_selected = false;

						updateSelected();
					}
					else
					{
						if (!m_selected)
						{
							m_selected = true;
							
							updateSelected();
						}
					}
				}

				break;
			}
			}
		}
	}
}

//=============================================================================
// Function: void processEntityDestroy(EntityDestroyMessage*)
// Description:
// Checks to see if the destroy message is for either its
// background or text and destroys it.
// Parameters:
// EntityDestroyMessage *destroyMsg - The destroy message to process.
//=============================================================================
void UIButton::processEntityDestroy(EntityDestroyMessage *destroyMsg)
{
	if (destroyMsg)
	{
		if (m_background->getEntityID() == destroyMsg->m_entityID)
		{
			delete m_background;
			m_background = NULL;
		}

		if (m_textUI->getEntityID() == destroyMsg->m_entityID)
		{
			delete m_textUI;
			m_textUI = NULL;
		}
	}
}

void UIButton::updateSelected()
{
	if (m_selected)
	{
		if (m_background)
		{
			SDL_Color greyedOut{ 100, 100, 100, 255 };
			m_background->setVisualEffect(TextureEffect::EFFECT_COLOR_FADE,
				greyedOut, 
				0, 
				0);
		}
	}
	else
	{
		if (m_background)
		{
			m_background->stopVisualEffect();
		}
	}
}