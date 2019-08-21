#pragma once
//==========================================================================================
// File Name: IMessage.h
// Author: Brian Blackmon
// Date Created: 5/13/2019
// Purpose: 
// The abstract base class for messages. 
//==========================================================================================
class IMessage
{
public:
	enum MessageType
	{
		ERROR = -1,
		MOVE,
		COLLISION,
		STATE_CHANGE,
		VELOCITY_INCREASE,
		ANIMATION_CHANGE,
		ENTITY_DESTROY,
		INPUT,
		CAMERA_MOVE
	};

	IMessage(MessageType type)
		:m_type(type)
	{
	}
	virtual ~IMessage() {}
	
	MessageType type() { return m_type; }

private:
	MessageType m_type;
};

