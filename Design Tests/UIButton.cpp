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
	m_onPress(NULL),
	m_onRelease(NULL),
	m_onHold(NULL)
{
	if(buttonTexture != "")
	{
		SpriteComponent *sprite = RenderSystem::instance()->createSprite(entityID, buttonTexture, position);

		if(sprite)
		{
			RenderSystem::instance()->setSpriteLayer(entityID, RenderSystem::RENDER_UI_FOREGROUND);
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
// Function: void setRect(Rectangle*)
// Description:
// Sets the bounding box of the button to the specified rect.
// Parameters:
// Rectangle *rect - The rectangle to set the bounding box to.
//=============================================================================
void UIButton::setRect(Shape::Rectangle *rect)
{
	if (rect)
	{
		if (m_rect && m_rect != rect)
		{
			delete m_rect;

			m_rect = NULL;
		}

		m_rect = rect;
		m_rect->setCenter(m_position.getX(), m_position.getY());

		SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

		if (sprite)
		{
			sprite->setRenderSize(m_rect->width(), m_rect->height());
		}
	}
}

//=============================================================================
// Function: void setText(string, Font*, SDL_Color)
// Description:
// Sets the text of the button to the specified text.
// Parameters:
// string text - The text to set the button text to.
// Font *font - The font to use for the text.
// SDL_Color color - The color to use for the text.
//=============================================================================
void UIButton::setText(string text, Font *font, SDL_Color color)
{
	if(text != "" && font)
	{
		Uint32 wrapWidth = 0;

		if (m_rect)
		{
			int border = 20;
			wrapWidth = m_rect->width() - border;
		}

		TextComponent *textComp = 
			RenderSystem::instance()->createTextComponent(m_entityID,
														  text,
														  font,
														  color,
														  wrapWidth,
														  m_position);
	}
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the button.
// Parameters:
// int width - The new button width.
//=============================================================================
void UIButton::setWidth(int width)
{
	if (0 < width)
	{
		m_rect->setWidth(width);

		SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

		if (sprite)
		{
			SDL_Rect clip = sprite->renderSize();

			sprite->setRenderSize(m_rect->width(), clip.h);
		}

		TextComponent *text = RenderSystem::instance()->getText(m_entityID);

		if (text)
		{
			text->setWrapWidth(width - 20);
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the button.
// Parameters:
// int height - The new button height.
//=============================================================================
void UIButton::setHeight(int height)
{
	if (0 < height)
	{
		m_rect->setHeight(height);

		SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

		if (sprite)
		{
			SDL_Rect clip = sprite->renderSize();

			sprite->setRenderSize(clip.w, m_rect->height());
		}
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state.
// Parameters:
// bool active - The active state to set.
//=============================================================================
void UIButton::setActive(bool active)
{
	if (m_active != active)
	{
		if (m_active)
		{
			m_pressed = false;
			m_wasPressed = false;
		}

		m_active = active;
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
// Function: void setVisible(bool)
// Description:
// Sets the visiblity of the button.
// Parameters:
// bool visible - The visibility of the button.
//=============================================================================
void UIButton::setVisible(bool visible)
{
	SpriteComponent *sprite = RenderSystem::instance()->getSprite(m_entityID);

	if(sprite)
	{
		sprite->setVisible(visible);
	}

	TextComponent *textComp = RenderSystem::instance()->getText(m_entityID);

	if(textComp)
	{
		textComp->setVisible(visible);
	}

	m_visible = visible;
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
	if (m_active)
	{
		if(getPressed())
		{
			if(m_onPress)
			{
				m_onPress();
			}
		}
		else if(getReleased())
		{
			if(m_onRelease)
			{
				m_onRelease();
			}
		}
		else if(getHeld())
		{
			if(m_onHold)
			{
				m_onHold();
			}
		}
	}

	m_wasPressed = m_pressed;
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
			if (m_rect)
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
			}

			break;
		}
		}
	}

	switch (message->type())
	{
	case IMessage::MOVE:
	{
		MoveMessage *move = static_cast<MoveMessage*>(message);

		if (move->m_entityID == m_entityID)
		{
			setPosition(move->m_newPosition);
		}

		break;
	}
	}
}

//=============================================================================
// Function: void setOnPress(functionPtr)
// Description:
// Sets the current on press function to the specified function.
// Parameters:
// functionPtr onPress - The pointer to the function to run when 
// the button is pressed.
//=============================================================================
void UIButton::setOnPress(functionPtr onPress)
{
	m_onPress = onPress;
}

//=============================================================================
// Function: void setOnRelease(functionPtr)
// Description:
// Sets the current on release function to the specified function.
// Parameters:
// functionPtr onRelease - The pointer containing the function to run on
// release.
//=============================================================================
void UIButton::setOnRelease(functionPtr onRelease)
{
	m_onRelease = onRelease;
}

//=============================================================================
// Function: void setOnHold(functionPtr)
// Description:
// Sets the current on hold function to the specified function.
// Parameters:
// functionPtr onHold - The pointer containing the function to run on
// button held.
//=============================================================================
void UIButton::setOnHold(functionPtr onHold)
{
	m_onHold = onHold;
}