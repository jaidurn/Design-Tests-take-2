#include "PlayerMoveState.h"
#include "VelocityIncreaseMessage.h"
#include "StateChangeMessage.h"
#include "CollisionMessage.h"
#include "EntityDestroyMessage.h"
#include "EntitySystem.h"
#include "InputSystem.h"
#include "Controller.h"
#include "Rotation.h"
#include <cmath>

PlayerMoveState::PlayerMoveState(int entityID, std::string stateName, float walkSpeed)
	:PlayerState(entityID, stateName), m_walkSpeed(walkSpeed)
{
}


PlayerMoveState::~PlayerMoveState()
{
}

//=============================================================================
// Function: void enter()
// Description:
// Enters the player move state.
//=============================================================================
void PlayerMoveState::enter()
{
	sendAnimationChangeMessage(m_name, AnimationComponent::DIR_NONE, 0);
}

//=============================================================================
// Function: void update()
// Description:
// Handles the player movement.
//=============================================================================
void PlayerMoveState::update()
{
	InputComponent *input = InputSystem::instance()->getInputComponent(m_entityID);

	if(input)
	{
		if(input->xLeftAxis() == 0 && input->yLeftAxis() == 0)
		{
			sendStateChangeMessage("Idle");
		}
		else
		{
			float directionCount = 360.0f / 4.0f;
			int direction = (int)round(input->leftAxisAngle() / directionCount);

			if(4 <= direction)
			{
				direction = 0;
			}

			float xMove = m_walkSpeed * input->xLeftAxis();
			float yMove = m_walkSpeed * input->yLeftAxis();

			float magnitude = sqrt((xMove * xMove) + (yMove * yMove));

			if (m_walkSpeed < magnitude)
			{
				float xFix = magnitude - abs(xMove);
				float yFix = magnitude - abs(yMove);

				if(xMove < 0)
				{
					xMove += yFix;
				}
				else
				{
					xMove -= yFix;
				}

				if(yMove < 0)
				{
					yMove += xFix;
				}
				else
				{
					yMove -= xFix;
				}
			}

			sendAnimationChangeMessage(m_name, (AnimationComponent::Direction)direction, -1);
			sendVelocityIncreaseMsg(xMove, yMove);
		}
	}
}

//=============================================================================
// Function: void exit()
// Description:
// Plays all the exit stuff.
//=============================================================================
void PlayerMoveState::exit()
{

}

//=============================================================================
// Function: void processMessage(IMessage *message)
// Description:
// Processes system messages passed in.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void PlayerMoveState::processMessage(IMessage *message)
{
	if (message)
	{
		switch (message->type())
		{
		}
	}
}

//=============================================================================
// Function: void sendVelocityIncreaseMsg(float, float)
// Description:
// Sends a message to increase the velocity.
// Parameters:
// float xSpeed - The amount to increase the x velocity by.
// float ySpeed - The amount to increase the y velocity by.
//=============================================================================
void PlayerMoveState::sendVelocityIncreaseMsg(float xSpeed, float ySpeed)
{
	VelocityIncreaseMessage *message = new VelocityIncreaseMessage(m_entityID, xSpeed, ySpeed, xSpeed, ySpeed);

	MessageSystem::instance()->pushMessage(message);
}

