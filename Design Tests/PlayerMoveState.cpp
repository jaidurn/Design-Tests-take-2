#include "PlayerMoveState.h"
#include "VelocityIncreaseMessage.h"
#include "StateChangeMessage.h"
#include "InputSystem.h"
#include "Controller.h"
#include "Rotation.h"
#include <cmath>

PlayerMoveState::PlayerMoveState(int entityID, std::string stateName, float walkSpeed)
	:IState(entityID), m_name(stateName), m_walkSpeed(walkSpeed)
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
	sendAnimationChangeMessage(m_name, 0);
}

//=============================================================================
// Function: void update()
// Description:
// Handles the player movement.
//=============================================================================
void PlayerMoveState::update()
{
	Input::InputComponent *input = Input::InputSystem::instance()->getInputComponent(m_entityID);

	if(input)
	{
		if(input->xAxis() == 0 && input->yAxis() == 0)
		{
			sendStateChangeMessage("Idle");
		}
		else
		{
			float eighthOfCircle = 360.0f / 4.0f;
			int direction = (int)round(input->axisAngle() / eighthOfCircle);

			if(4 < direction)
			{
				direction = 0;
			}

			float xMove = m_walkSpeed * input->xAxis();
			float yMove = m_walkSpeed * input->yAxis();

			float absX = xMove;
			float absY = yMove;

			if (absX < 0) { absX *= -1; }
			if (absY < 0) { absY *= -1; }

			if(m_walkSpeed < (absX + absY))
			{
				float totalDifference = (absX + absY) - m_walkSpeed;

				if(totalDifference < m_walkSpeed)
				{
					if(xMove < 0)
					{
						xMove += totalDifference / 2.0f;
					}
					else
					{
						xMove -= totalDifference / 2.0f;
					}

					if(yMove < 0)
					{
						yMove += totalDifference / 2.0f;
					}
					else
					{
						yMove -= totalDifference / 2.0f;
					}
				}
				else
				{
//					std::cout << "X speed: " << xMove
//						<< " Y speed: " << yMove
//						<< " total: " << totalDifference << std::endl;
				}
			}

			sendAnimationChangeMessage(m_name, direction);
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

