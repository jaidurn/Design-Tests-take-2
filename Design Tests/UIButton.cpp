#include "UIButton.h"
#include "UIText.h"
#include "UIGraphic.h"
#include "MessageSystem.h"
#include "EntityDestroyMessage.h"
#include "PhysicsSystem.h"
#include "InputMessage.h"
#include "InputSystem.h"
#include "Mouse.h"

UIButton::UIButton(int entityID,
	Vector2D position,
	string name,
	bool toggle)
	:UIComponent(entityID, UI_BUTTON, position, name),
	m_textUI(NULL),
	m_background(NULL),
	m_toggle(toggle),
	m_pressed(false),
	m_wasPressed(false)
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

	if (m_rect && m_textUI)
	{
		Vector2D textOffset = m_textUI->getPosition() - m_rect->center();

		m_textUI->setPosition(position + textOffset);

		m_rect->setCenter(position);
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
			int widthDifference = width - m_rect->width();

			m_textUI->setWidth(m_textUI->getWidth() - widthDifference);

			// Fix the x position of the text
			Vector2D textPosDiff = m_textUI->getPosition() - m_rect->center();

			textPosDiff.setX(textPosDiff.getX() - ((float)widthDifference / 2.0f));

			m_textUI->setPosition(textPosDiff);
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
			int heightDifference = height - m_rect->height();

			m_textUI->setWidth(m_textUI->getHeight() - heightDifference);

			// Fix the Y position of the text.
			Vector2D textPosDiff = m_textUI->getPosition() - m_rect->center();

			textPosDiff.setY(textPosDiff.getY() - ((float)heightDifference / 2.0f));

			m_textUI->setPosition(textPosDiff);
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

		m_textUI->setPosition(m_textUI->getPosition() + m_rect->center());
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

				if (button->m_deviceType == InputDevice::MOUSE)
				{
					InputDevice *inputDevice = InputSystem::instance()->getDevice(button->m_deviceID);

					Input::Mouse *mouse = static_cast<Input::Mouse*>(inputDevice);

					Vector2D mousePosition(mouse->getX(), mouse->getY());

					Camera2D *camera = RenderSystem::instance()->camera();

					// Scale the mouse position to the proper spot.
					if (camera)
					{
						mousePosition.setX(mousePosition.getX() + camera->getX());
						mousePosition.setY(mousePosition.getY() + camera->getY());
					}

					if (collisionSys->pointInsideRect(m_rect, (int)mousePosition.getX(), (int)mousePosition.getY()))
					{
						if (button->m_button == SDL_BUTTON_LEFT)
						{
							if (button->m_pressed)
							{
								m_pressed = true;
							}
							else
							{
								m_pressed = false;
							}
						}
					}
				}

				break;
			}
			case InputMessage::INPUT_MOVE:
			{
				InputMoveMessage *inputMove = static_cast<InputMoveMessage*>(inputMsg);

				if (inputMove->m_deviceType == InputDevice::MOUSE)
				{
					InputDevice *inputDevice = InputSystem::instance()->getDevice(inputMove->m_deviceID);

					if (inputDevice->type() == InputDevice::MOUSE)
					{
						Input::Mouse *mouse = static_cast<Input::Mouse*>(inputDevice);

						Vector2D mousePosition(mouse->getX(), mouse->getY());

						Camera2D *camera = RenderSystem::instance()->camera();

						if (camera)
						{
							mousePosition.setX(mousePosition.getX() + camera->getX());
							mousePosition.setY(mousePosition.getY() + camera->getY());
						}

						if (!collisionSys->pointInsideRect(m_rect, (int)mousePosition.getX(), (int)mousePosition.getY()))
						{
							m_pressed = false;
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