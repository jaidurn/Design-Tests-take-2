#pragma once
//==========================================================================================
// File Name: DoorLogicComponent.h
// Author: Brian Blackmon
// Date Created: 8/8/2019
// Purpose: 
// Holds the logic for door interactions.
//==========================================================================================
#include "LogicComponent.h"
#include <map>

class Door;

class DoorLogicComponent :
	public LogicComponent
{
public:
	enum DoorStates
	{
		DOOR_OPEN,
		DOOR_CLOSED,
		DOOR_LOCKED
	};

	DoorLogicComponent(int entityID, Door *door);
	~DoorLogicComponent();

	virtual void update();

	virtual void processMessage(IMessage *message);

	void setDoor(Door *door);

private:
	Door *m_door;
	DoorStates m_currentState;

	void openDoor();

	// Not needed.
	virtual void addState(std::string stateName, IState *state) {}
	virtual void removeState(std::string stateName) {}
	virtual IState *getState(std::string stateName) { return NULL; }
	virtual IState *currentState() { return NULL; }
	virtual void changeState(std::string stateName) {}
	virtual void cleanUp() {}

};

