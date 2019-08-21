#include "MessageSystem.h"
#include "CollisionMessage.h"
#include "MoveMessage.h"
#include "StateChangeMessage.h"
#include "VelocityIncreaseMessage.h"
#include "AnimationChangeMessage.h"
#include "EntityDestroyMessage.h"
#include "InputMessage.h"
#include "CameraMoveMessage.h"
#include <SDL.h>
#include <cmath>
#include <iostream>

//=============================================================================
// Function: bool pollMessage(IMessage)
// Description:
// Polls the event to see if there's a system message in the queue.
// Parameters:
// IMessage &message - The message to fill.
// Output:
// Returns true if there's a message.
// Returns false if there isn't.
//=============================================================================
bool MessageSystem::pollMessage(IMessage *& message)
{
	bool messageRetrieved = false;

	int msgIndex = getNextUnprocessedMessage();

	if(msgIndex != -1)
	{
		if (m_messages[msgIndex])
		{
			message = m_messages[msgIndex];
			m_messageFlag[msgIndex] = true;

			messageRetrieved = true;
		}
	}

	if (!messageRetrieved)
	{
		message = NULL;
	}

	return messageRetrieved;
}

//=============================================================================
// Function: void pushMessage(IMessage)
// Description:
// Pushes a message onto the queue.
// Parameters:
// IMessage message - The message to push.
//=============================================================================
void MessageSystem::pushMessage(IMessage *message)
{
	if(!combineMessage(message))
	{
		if(m_messages.size() < (unsigned int)m_MAX_MESSAGES)
		{
			m_messages.push_back(message);
			m_messageFlag.push_back(false);
		}
		else
		{
			// Find the next available message.
			int index = getFirstProcessedMessage();

			if(0 <= index)
			{
				delete m_messages[index];
				m_messages[index] = NULL;

				if (message)
				{
					m_messages[index] = message;
					m_messageFlag[index] = false;
				}
			}
			else if(index == -1)
			{
				delete message;
				message = NULL;
			}
		}
	}

}

//=============================================================================
// Function: bool peekMessage(IMessage *& message)
// Description:
// Loops through the message queue and peeks through the messages.
// Parameters:
// IMessage *& message - The message to fill the data of.
// Output:
// bool - Returns true on message found
// Returns false if no message.
//=============================================================================
bool MessageSystem::peekMessage(IMessage *& message, int startingIndex)
{
	bool messageFound = false;

	if(0 <= startingIndex && (unsigned int)startingIndex < m_messages.size())
	{
		if(m_messageFlag[startingIndex] == false)
		{
			message = m_messages[startingIndex];
			messageFound = true;
		}
	}

	return messageFound;
}

//=============================================================================
// Function: void flushMessages()
// Description:
// Flushes the message queue, deleting all of the current messages.
//=============================================================================
void MessageSystem::flushMessages()
{
	for(unsigned int i = 0; i < m_messages.size(); i++)
	{
		delete m_messages[i];
	}

	m_messages.resize(0);

	m_messageFlag.clear();
	m_messageFlag.resize(0);
}

//=============================================================================
// Function: bool combineMessage(IMessage&)
// Description:
// Looks through the message queue and sees if it can merge the message with
// any of the existing messages.
// Parameters:
// IMessage &message - The message to try to combine.
// Output:
// Returns true on successful combining.
// Returns false on failure.
//=============================================================================
bool MessageSystem::combineMessage(IMessage *message)
{
	bool combined = false;

	switch(message->type())
	{
	case IMessage::MessageType::COLLISION:
	{
		combined = combineCollision(message);
		break;
	}
	case IMessage::MessageType::MOVE:
	{
		combined = combineMove(message);
		break;
	}
	case IMessage::MessageType::VELOCITY_INCREASE:
	{
		combined = combineVelIncrease(message);
		break;
	}
	case IMessage::MessageType::ANIMATION_CHANGE:
	{
		combined = combineAnimationChange(message);
		break;
	}
	case IMessage::ENTITY_DESTROY:
	{
		combined = combineEntityDestroy(message);
		break;
	}
	case IMessage::INPUT:
	{
		combined = combineInput(message);
		break;
	}
	case IMessage::CAMERA_MOVE:
	{
		combined = combineCameraMove(message);

		break;
	}
	}

	return combined;
}

//=============================================================================
// Function: int getNextUnprocessedMessage()
// Description:
// Gets the next message that hasn't been processed.
// Output:
// Returns the index number of the unprocessed message.
// Returns -1 on error.
//=============================================================================
int MessageSystem::getNextUnprocessedMessage()
{

	if (0 < m_messages.size())
	{
		while (m_currentPollIndex < (int)m_messages.size() && m_messageFlag[m_currentPollIndex])
		{
			m_currentPollIndex++;
		}

		if (m_currentPollIndex == (int)m_messages.size())
		{
			m_currentPollIndex = 0;
			return -1;
		}
		else if(m_messages[m_currentPollIndex] == NULL)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return m_currentPollIndex;
}

//=============================================================================
// Function: int getFirstProcessedMessage()
// Description:
// Finds the first message in the vector that has been processed by 
// the poll loop.
// Output:
// Returns the index of the processed message on success.
// Returns -1 on failure.
//=============================================================================
int MessageSystem::getFirstProcessedMessage()
{
	int messageIndex = -1;

	if (0 < m_messages.size())
	{
		if(!m_messageFlag[m_currentPushIndex])
		{
			int prevPushIndex = m_currentPushIndex;
			
			m_currentPushIndex++;

			if(m_currentPushIndex == (int)m_messages.size())
			{
				m_currentPushIndex = 0;
			}

			while(m_currentPushIndex != prevPushIndex && !m_messageFlag[m_currentPushIndex])
			{
				m_currentPushIndex++;

				if(m_currentPushIndex == (int)m_messages.size())
				{
					m_currentPushIndex = 0;
				}
			}

			if(m_currentPushIndex != prevPushIndex)
			{
				messageIndex = m_currentPushIndex;
			}
		}
		else
		{
			messageIndex = m_currentPushIndex;
		}
	}

	if(messageIndex != -1)
	{
		delete m_messages[messageIndex];
		m_messages[messageIndex] = NULL;
	}

	return messageIndex;
}

//=============================================================================
// Function: void resetMessageFlag(int)
// Description:
// Resets the message flag for the specified message, setting it to false.
// This means the message hasn't been processed.
// Parameters:
// int index - The index of the flag to reset.
//=============================================================================
void MessageSystem::resetMessageFlag(int index)
{
	if(0 <= index && index < (int)m_messageFlag.size())
	{
		m_messageFlag[index] = false;

		if(index < m_currentPollIndex)
		{
			m_currentPollIndex = index;
		}
	}
}

//=============================================================================
// Function: bool combineCollision(IMessage*)
// Description:
// Combines the collision message with another compatible collision message
// if possible.
// Parameters:
// IMessage *message - The message to combine.
// Output:
// Returns true if the message was combined.
// Returns false if the message wasn't combined.
//=============================================================================
bool MessageSystem::combineCollision(IMessage *message)
{
	bool combined = false;

	for (unsigned int i = 0; i < m_messages.size(); i++)
	{
		if(m_messages[i]->type() == IMessage::MessageType::COLLISION)
		{
			CollisionMessage *messageToCombine = static_cast<CollisionMessage*>(message);
			CollisionMessage *currentMessage = static_cast<CollisionMessage*>(m_messages[i]);

			if(currentMessage->m_entityID == messageToCombine->m_entityID)
			{
				if(currentMessage->m_collidingID == messageToCombine->m_collidingID)
				{
					currentMessage->m_position = messageToCombine->m_position;
					
					resetMessageFlag(i);

					combined = true;

					// Delete the created message
					delete message;
					message = NULL;
				}
			}
		}

		if(combined)
		{
			break;
		}
	}

	return combined;
}

//=============================================================================
// Function: bool combineMove(IMessage*)
// Description:
// Trys to combine the move message with an already existing move message.
// Parameters:
// IMessage *message - The message to try to combine.
// Output:
// Returns true if the message was combined.
// Returns false if the message failed to be combined.
//=============================================================================
bool MessageSystem::combineMove(IMessage *message)
{
	bool combined = false;

	if (message->type() == IMessage::MOVE)
	{
		for (unsigned int i = 0; i < m_messages.size(); i++)
		{
			if (m_messages[i]->type() == IMessage::MessageType::MOVE)
			{
				// TODO: Remember. This takes away the system's "memory"
				// of moves that are made. If you want to track them and store
				// them, make a separate tool for that.
				MoveMessage *combineMove = static_cast<MoveMessage*>(message);
				MoveMessage *currentMove = static_cast<MoveMessage*>(m_messages[i]);

				if (currentMove->m_entityID == combineMove->m_entityID)
				{
					currentMove->m_oldPosition = combineMove->m_oldPosition;
					currentMove->m_newPosition = combineMove->m_newPosition;

					resetMessageFlag(i);

					combined = true;

					delete message;
					message = NULL;
				}
			}

			if (combined)
			{
				break;
			}
		}
	}

	return combined;
}

//=============================================================================
// Function: bool combineVelIncrease(IMessage*)
// Description:
// Trys to combine the velocity increase message with an 
// already existing velocity increase message.
// Parameters:
// IMessage *message - The message to try to combine.
// Output:
// Returns true if the message was combined.
// Returns false if the message failed to be combined.
//=============================================================================
bool MessageSystem::combineVelIncrease(IMessage *message)
{
	bool combined = false;

	for (unsigned int i = 0; i < m_messages.size(); i++)
	{
		if (m_messages[i]->type() == IMessage::MessageType::VELOCITY_INCREASE)
		{
			VelocityIncreaseMessage *messageToCombine = static_cast<VelocityIncreaseMessage*>(message);
			VelocityIncreaseMessage *currentMessage = static_cast<VelocityIncreaseMessage*>(m_messages[i]);

			if (currentMessage->m_entityID == messageToCombine->m_entityID)
			{

				if (m_messageFlag[i] == true)
				{
					currentMessage->m_xIncrease = messageToCombine->m_xIncrease;
					currentMessage->m_yIncrease = messageToCombine->m_yIncrease;
					currentMessage->m_xMaxSpeed = messageToCombine->m_xMaxSpeed;
					currentMessage->m_yMaxSpeed = messageToCombine->m_yMaxSpeed;

					resetMessageFlag(i);

					combined = true;

					// Delete the created message
					delete message;
					message = NULL;
				}
			}
		}

		if (combined)
		{
			break;
		}
	}

	return combined;
}

//=============================================================================
// Function: bool combineAnimationChange(IMessage*)
// Description:
// Trys to combine the animation change message with an already 
// existing animation change message.
// Parameters:
// IMessage *message - The message to try to combine.
// Output:
// Returns true if the message was combined.
// Returns false if the message failed to be combined.
//=============================================================================
bool MessageSystem::combineAnimationChange(IMessage *message)
{
	bool combined = false;

	AnimationChangeMessage *animation = static_cast<AnimationChangeMessage*>(message);

	for(unsigned int i = 0; i < m_messages.size(); i++)
	{
		if(m_messages[i]->type() == IMessage::ANIMATION_CHANGE)
		{
			AnimationChangeMessage *combine = static_cast<AnimationChangeMessage*>(m_messages[i]);

			if (animation->m_entityID == combine->m_entityID)
			{
				combine->m_name = animation->m_name;
				combine->m_frame = animation->m_frame;
				combine->m_direction = animation->m_direction;

				resetMessageFlag(i);

				combined = true;

				delete message;
				message = NULL;
			}
		}

		if(combined)
		{
			break;
		}
	}

	return combined;
}

//=============================================================================
// Function: bool combineEntityDestroy(IMessage*)
// Description:
// Checks to see if there is already a destroy message for an entity.
// If so, it combines the current message with the existing message.
// Parameters:
// IMessage *message - The message to try to combine.
// Output:
// Returns true on success.
// Returns false on failure.
//=============================================================================
bool MessageSystem::combineEntityDestroy(IMessage *message)
{
	bool combined = false;

	if(message)
	{
		EntityDestroyMessage *destroy = static_cast<EntityDestroyMessage*>(message);

		if(destroy)
		{
			for(unsigned int i = 0; i < m_messages.size(); i++)
			{
				if(m_messages[i]->type() == IMessage::ENTITY_DESTROY)
				{
					EntityDestroyMessage *current = static_cast<EntityDestroyMessage*>(m_messages[i]);

					if(current)
					{
						if(current->m_entityID == destroy->m_entityID)
						{
							resetMessageFlag(i);

							combined = true;

							delete message;
							message = NULL;
						}
					}
				}

				if(combined)
				{
					break;
				}
			}
		}
	}

	return combined;
}

//=============================================================================
// Function: bool combineInput(IMessage*)
// Description:
// Trys to combine the input message with an already existing input message.
// Parameters:
// IMessage *message - The message to try to combine.
// Output:
// Returns true if the message was combined.
// Returns false if the message failed to be combined.
//=============================================================================
bool MessageSystem::combineInput(IMessage *message)
{
	bool combined = false;

	if(message->type() == IMessage::INPUT)
	{
		InputMessage *first = static_cast<InputMessage*>(message);

		for (unsigned int i = 0; i < m_messages.size(); i++)
		{
			if (m_messages[i]->type() == IMessage::INPUT)
			{
				InputMessage *second = static_cast<InputMessage*>(m_messages[i]);

				if (first->m_inputType == second->m_inputType &&
					first->m_deviceID == second->m_deviceID)
				{
					// Do final checks to make sure the message can be combined.
					switch (first->m_inputType)
					{
					case InputMessage::INPUT_AXIS:
					{
						InputAxisMessage *firstAxis = static_cast<InputAxisMessage*>(first);
						InputAxisMessage *secondAxis = static_cast<InputAxisMessage*>(second);

						if(firstAxis->m_axis == secondAxis->m_axis)
						{
							secondAxis->m_axisMovement = firstAxis->m_axisMovement;

							combined = true;
						}

						break;
					}
					case InputMessage::INPUT_BUTTON:
					{
						InputButtonMessage *firstButton = static_cast<InputButtonMessage*>(first);
						InputButtonMessage *secondButton = static_cast<InputButtonMessage*>(second);

						if(firstButton->m_button == secondButton->m_button)
						{
							secondButton->m_pressed = firstButton->m_pressed;

							combined = true;
						}

						break;
					}
					case InputMessage::INPUT_MOVE:
					{
						InputMoveMessage *firstMove = static_cast<InputMoveMessage*>(first);
						InputMoveMessage *secondMove = static_cast<InputMoveMessage*>(second);

						secondMove->m_x = firstMove->m_x;
						secondMove->m_y = firstMove->m_y;

						combined = true;

						break;
					}
					}

					// If we've successfully combined the message
					// reset and exit.
					if(combined)
					{
						resetMessageFlag(i);

						delete message;
						message = NULL;

						break;
					}
				}
			}
		}
	}

	return combined;
}

//=============================================================================
// Function: bool combineCameraMove(IMessage*)
// Description:
// Combines the move message with an existing move message.
// Parameters:
// IMessage *message - The message to try to combine.
// Output:
// Returns true on success.
// Returns false on failure.
//=============================================================================
bool MessageSystem::combineCameraMove(IMessage *message)
{
	bool combined = false;

	if(message->type() == IMessage::CAMERA_MOVE)
	{
		CameraMoveMessage *combineAttempt = static_cast<CameraMoveMessage*>(message);

		for(unsigned int i = 0; i < m_messages.size(); i++)
		{
			if(m_messages[i]->type() == IMessage::CAMERA_MOVE)
			{
				CameraMoveMessage *combineWith = static_cast<CameraMoveMessage*>(m_messages[i]);

				combineWith->m_oldPosition = combineAttempt->m_oldPosition;
				combineWith->m_newPosition = combineAttempt->m_newPosition;

				resetMessageFlag(i);

				delete message;
				message = NULL;

				combined = true;

				break;
			}
		}

	}

	return combined;
}