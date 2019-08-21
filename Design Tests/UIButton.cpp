#include "UIButton.h"
#include <iostream>
#include "IMessage.h"
#include "InputMessage.h"
#include "InputSystem.h"
#include "CameraMoveMessage.h"
#include "Mouse.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "MoveMessage.h"
#include "MessageSystem.h"
#include <cmath>

UIButton::UIButton(int entityID, Vector2D position, Shape::Rectangle *rect, std::string buttonTexture)
	:UIComponent(UI_BUTTON, entityID, position), 
	m_wasPressed(false),
	m_pressed(false),
	m_rect(rect),
	m_update(NULL)
{
	if(buttonTexture != "")
	{
		SpriteComponent *sprite = RenderSystem::instance()->createSprite(entityID, buttonTexture, position);

		if(sprite)
		{
			RenderSystem::instance()->setSpriteLayer(entityID, RenderSystem::RENDER_UI);
			sprite->setScaleable(false);
		}
	}
}


UIButton::~UIButton()
{
	if (m_rect)
	{
		delete m_rect;
		m_rect = NULL;
	}
}

//=============================================================================
// Function:
// Description:
// Gets the state of the button. If the button wasn't pressed before, and
// is pressed now, it's pressed. Otherwise the button isn't pressed.
// Output:
// Returns true if the button was pressed.
// Returns false if the button was not pressed.
//=============================================================================
bool UIButton::getPressed()
{
	bool pressed = false;

	if(!m_wasPressed && m_pressed)
	{
		pressed = true;
	}

	return pressed;
}

//=============================================================================
// Function: bool getReleased()
// Description:
// Checks if the button was released or not.
// Output:
// Returns true if the button was just released.
// Returns false if the button is held, or hasn't been pressed.
//=============================================================================
bool UIButton::getReleased()
{
	bool released = false;

	if(m_wasPressed && !m_pressed)
	{
		released = true;
	}

	return released;
}

//=============================================================================
// Function: bool getHeld()
// Description:
// Checks if the button is being held down.
// Output:
// Returns true if the button has been held down.
// Returns false if the button isn't being held.
//=============================================================================
bool UIButton::getHeld()
{
	bool held = false;

	if(m_wasPressed && m_pressed)
	{
		held = true;
	}

	return held;
}

//=============================================================================
// Function: void setPressed(bool)
// Description:
// Updates the state of the button and the previous state of the button.
// Parameters:
// bool pressed - The state to set the button to.
//=============================================================================
void UIButton::setPressed(bool pressed)
{
	m_wasPressed = m_pressed;
	m_pressed = pressed;
}

//=============================================================================
// Function:
// Description:
//
// Parameters:
//
// Output:
// 
//=============================================================================
void UIButton::setRect(Shape::Rectangle *rect)
{
	if (rect)
	{
		m_rect = rect;
		m_rect->setCenter(m_position.getX(), m_position.getY());
	}
}

//=============================================================================
// Function: void setPosition(Vector2D)
// Description:
// Updates the position of the button and its rect.
// Parameters:
// Vector2D position - The position to set the button to.
//=============================================================================
void UIButton::setPosition(Vector2D position)
{
	if (m_rect)
	{
		m_rect->setCenter(position.getX(), position.getY());
	}

	MoveMessage *move = new MoveMessage(m_entityID, m_position, position);

	MessageSystem::instance()->pushMessage(move);

	m_position = position;
}

//=============================================================================
// Function: void update(float)
// Description:
// Updates the logic of the button.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void UIButton::update(float delta)
{
	m_wasPressed = m_pressed;

	if (m_active)
	{
		if (m_update)
		{
			m_update();
		}
	}
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes the message passed to it.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIButton::processMessage(IMessage *message)
{
	if (m_active)
	{
		switch (message->type())
		{
		case IMessage::CAMERA_MOVE:
		{
			if(m_docked)
			{
				CameraMoveMessage *move = static_cast<CameraMoveMessage*>(message);

				Camera2D *camera = RenderSystem::instance()->camera();

				float oldX = (float)(round(m_position.getX()) - round(move->m_oldPosition.getX()));
				float oldY = (float)(round(m_position.getY()) - round(move->m_oldPosition.getY()));

				Vector2D newPosition{ oldX + (move->m_newPosition.getX()),
									  oldY + (move->m_newPosition.getY()) };

				setPosition(newPosition);
			}

			break;
		}
		case IMessage::INPUT:
		{
			CollisionSystem *collisionSys = PhysicsSystem::instance()->collisionSystem();

			InputMessage *input = static_cast<InputMessage*>(message);

			switch (input->m_inputType)
			{
			case InputMessage::INPUT_BUTTON:
			{
				InputButtonMessage *button = static_cast<InputButtonMessage*>(input);

				if (button->m_deviceType == InputDevice::MOUSE && button->m_button == SDL_BUTTON_LEFT)
				{
					InputDevice *device = InputSystem::instance()->getDevice(button->m_deviceID);

					if (device)
					{
						if (device->type() == InputDevice::MOUSE)
						{
							Input::Mouse *mouse = static_cast<Input::Mouse*>(device);

							Vector2D scaledPoint{ mouse->getX(), mouse->getY() };

							Camera2D *camera = RenderSystem::instance()->camera();

							if (camera)
							{
								// TODO: FIND OUT WHY THE SCALING DOESN'T WORK
								scaledPoint.setX(scaledPoint.getX() + (camera->getX()));
								scaledPoint.setY(scaledPoint.getY() + (camera->getY()));
							}

							if (collisionSys->pointInsideRect(m_rect, (int)scaledPoint.getX(), (int)scaledPoint.getY()))
							{
								m_wasPressed = m_pressed;

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
				}

				break;
			}
			case InputMessage::INPUT_MOVE:
			{
				InputMoveMessage *move = static_cast<InputMoveMessage*>(input);

				if (move->m_deviceType == InputDevice::MOUSE)
				{
					InputDevice *device = InputSystem::instance()->getDevice(move->m_deviceID);

					if (device)
					{
						if (device->type() == InputDevice::MOUSE)
						{
							Input::Mouse *mouse = static_cast<Input::Mouse*>(device);

							Vector2D scaledPoint{ mouse->getX(), mouse->getY() };

							Camera2D *camera = RenderSystem::instance()->camera();

							if (camera)
							{
								scaledPoint.setX(scaledPoint.getX() + (camera->getX()));
								scaledPoint.setY(scaledPoint.getY() + (camera->getY()));
							}

							if (!collisionSys->pointInsideRect(m_rect, scaledPoint.getX(), scaledPoint.getY()))
							{
								m_wasPressed = m_pressed;
								m_pressed = false;
							}
						}
					}
				}

				break;
			}
			}

			break;
		}
		}
	}
}

//=============================================================================
// Function: void setUpdateFunction(updatePtr)
// Description:
// Sets the update function to use.
// Parameters:
// updatePtr update - The new update function to use for the button.
//=============================================================================
void UIButton::setUpdateFunction(updatePtr update)
{
	if(update)
	{
		m_update = update;
	}
}