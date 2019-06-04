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
//	void peekMessage(SystemMessage &message);

private:
	const int m_MAX_MESSAGES = 100;
	std::vector<IMessage*> m_messages;
	std::vector<bool> m_messageFlag;

	MessageSystem()
	{
	}

	void flushMessages();
	bool combineMessage(IMessage *message);
	int getNextUnprocessedMessage();
	int getFirstProcessedMessage();

	// Message combining functions
	bool combineCollision(IMessage *message);
	bool combineMove(IMessage *message);
	bool combineVelIncrease(IMessage *message);
	bool combineAnimationChange(IMessage *message);
//	bool combinePhysic(SystemMessage &message);
};

