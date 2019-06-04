#pragma once
//==========================================================================================
// File Name: Message.h
// Author: Brian Blackmon
// Date Created: 4/23/2019
// Purpose: 
// A message header. Contains all sorts of messages.
//==========================================================================================
#include "Vector2D.h"
#include <string>

enum MessageType
{
	PHYSIC,
	MOVE,
	COLLISION,
	STATE_CHANGE
};

struct PhysicsMsg
{
	MessageType type;
	int entityID;
};

struct MoveMsg
{
	MessageType type;
	int entityID;
	Vector2D oldPosition;
	Vector2D newPosition;
};

struct CollisionMsg
{
	MessageType type;
	int entityID;
	int collidingEntityID;
	Vector2D position;
};

struct StateChangeMsg
{
	MessageType type;
	int entityID;
	const char *stateName;
};

union SystemMessage
{
	MessageType type;
	PhysicsMsg phys;
	MoveMsg move;
	CollisionMsg collision;
	StateChangeMsg state;

	SystemMessage()
	{
	}

	~SystemMessage()
	{
	}


};