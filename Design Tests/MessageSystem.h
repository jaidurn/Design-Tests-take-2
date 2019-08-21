#pragma once
//==========================================================================================
// File Name: MessageSystem.h
// Author: Brian Blackmon
// Date Created: 4/24/2019
// Purpose: 
// Holds and handles messages for the game.
//==========================================================================================
#include "IMessage.h"
#include <SDL.h>
#include <vector>

class MessageSystem
{
public:
	static MessageSystem* instance()
	{
		static MessageSystem *instance = new MessageSystem();

		return instance;
	}

	~MessageSystem()
	{
		flushMessages();
	}

	bool pollMessage(IMessage *& message);
	void pushMessage(IMessage *message);
	bool peekMessage(IMessage *& message, int startingIndex);

	int messageCount() { return (int)m_messages.size(); }

private:
	const int m_MAX_MESSAGES = 10000;
	int m_currentPollIndex;
	int m_currentPeekIndex;
	int m_currentPushIndex;
	std::vector<IMessage*> m_messages;
	std::vector<bool> m_messageFlag;

	MessageSystem()
		:m_currentPeekIndex(0), m_currentPollIndex(0), m_currentPushIndex(0)
	{
	}

	void flushMessages();
	bool combineMessage(IMessage *message);
	int getNextUnprocessedMessage();
	int getFirstProcessedMessage();

	void resetMessageFlag(int index);

	// Message combining functions
	bool combineCollision(IMessage *message);
	bool combineMove(IMessage *message);
	bool combineVelIncrease(IMessage *message);
	bool combineAnimationChange(IMessage *message);
	bool combineEntityDestroy(IMessage *message);
	bool combineInput(IMessage *message);
	bool combineCameraMove(IMessage *message);
//	bool combinePhysic(SystemMessage &message);
};

