#pragma once
//==========================================================================================
// File Name: PlayerMoveState.h
// Author: Brian Blackmon
// Date Created: 5/13/2019
// Purpose: 
// Handles the player movement.
//==========================================================================================
#include "PlayerState.h"
class PlayerMoveState :	public PlayerState
{
public:
	PlayerMoveState(int entityID, std::string stateName, float walkSpeed);
	virtual ~PlayerMoveState();

	virtual void enter();
	virtual void update();
	virtual void exit();
	
	virtual void processMessage(IMessage *message);

private:
	float m_walkSpeed;

	void sendVelocityIncreaseMsg(float xSpeed, float ySpeed);
};

