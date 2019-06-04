#include "MessageSystem.h"
#include "CollisionMessage.h"
#include "MoveMessage.h"
#include "StateChangeMessage.h"
#include "VelocityIncreaseMessage.h"
#include "AnimationChangeMessage.h"
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
		if(m_messages.size() < m_MAX_MESSAGES)
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
	static unsigned int messageIndex = 0;

	if (0 < m_messages.size())
	{
		messageIndex = 0;

		while (messageIndex < m_messages.size() && m_messageFlag[messageIndex])
		{
			messageIndex++;
		}

		if (messageIndex == m_messages.size())
		{
			messageIndex = 0;
			return -1;
		}
		else if(m_messages[messageIndex] == NULL)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return messageIndex;
}

int MessageSystem::getFirstProcessedMessage()
{
	static unsigned int messageIndex = 0;
	
	if (0 < m_messages.size())
	{
		messageIndex = 0;

		while (messageIndex < m_messages.size() && !m_messageFlag[messageIndex])
		{
			messageIndex++;
		}

		if (messageIndex == m_messages.size())
		{
			messageIndex = 0;
			return -1;
		}
		else
		{
			delete m_messages[messageIndex];
			m_messages[messageIndex] = NULL;
		}
	}
	else
	{
		return -1;
	}

	return messageIndex;
}

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
					m_messageFlag[i] = false;

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

bool MessageSystem::combineMove(IMessage *message)
{
	bool combined = false;

	for(unsigned int i = 0; i < m_messages.size(); i++)
	{
		if(m_messages[i]->type() == IMessage::MessageType::MOVE)
		{
			// TODO: Remember. This takes away the system's "memory"
			// of moves that are made. If you want to track them and store
			// them, make a separate tool for that.
			MoveMessage *combineMove = static_cast<MoveMessage*>(message);
			MoveMessage *currentMove = static_cast<MoveMessage*>(m_messages[i]);

			if(currentMove->m_entityID == combineMove->m_entityID)
			{
				currentMove->m_oldPosition = combineMove->m_oldPosition;
				currentMove->m_newPosition = combineMove->m_newPosition;

				m_messageFlag[i] = false;

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

					m_messageFlag[i] = false;

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

				m_messageFlag[i] = false;

				combined = true;

				delete message;
				message = NULL;
			}
		}
	}

	return combined;
}