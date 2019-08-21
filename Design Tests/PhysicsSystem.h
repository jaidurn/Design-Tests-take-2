#pragma once
//==========================================================================================
// File Name: PhysicsSystem.h
// Author: Brian Blackmon
// Date Created: 3/25/2019
// Purpose: 
// Handles the physics for the game.
//==========================================================================================
#include <map>
#include "CollisionSystem.h"
#include "VelocityComponent.h"
#include "MessageSystem.h"
#include "IMessage.h"

class PhysicsSystem
{
public:
	static PhysicsSystem *instance()
	{
		static PhysicsSystem *instance = new PhysicsSystem();

		return instance;
	}

	~PhysicsSystem();

	void update(float delta);

	CollisionSystem* collisionSystem()
	{
		return m_collisionSystem;
	}

	// Collision Functions
	CollisionComponent* createCollisionComponent(int ID, 
												 Shape::ShapeType shapeType,
												 int centerX,
												 int centerY);
	CollisionComponent* getCollisionComponent(int ID);

	// Velocity Functions
	VelocityComponent* getVelocityComponent(int ID);

	void processMessage(IMessage *message);

	void initCollisionSystem(int gridX, int gridY, int width, int height, int cellSize);

	bool hasLineOfSight(int entityID, int otherEntityID);

private:
	PhysicsSystem()
		:m_collisionSystem(NULL)
	{
	}

	std::map<int, VelocityComponent*> m_velocityComponents;
	CollisionSystem *m_collisionSystem;

	void sendMoveMessage(int entityID, Vector2D oldPosition, Vector2D newPosition);
	
	void removeVelocityComponent(int entityID);

	void cleanUp();
	void applyVelocity(float delta);
	void applyFriction(int entityID, float delta);
	Vector2D lerp(Vector2D goal, Vector2D current, float amount);
};

