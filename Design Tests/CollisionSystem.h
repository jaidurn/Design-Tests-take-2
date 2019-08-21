#pragma once
//==========================================================================================
// File Name: CollisionSystem.h
// Author: Brian Blackmon
// Date Created: 3/28/2019
// Purpose: 
// Handles all collisions for the program.
//==========================================================================================
#include <map>
#include "MessageSystem.h"
#include "CollisionComponent.h"
#include "Line.h"
#include "Grid.h"

class CollisionSystem
{
public:
	CollisionSystem(int gridX, int gridY, int width, int height, int cellSize);
	~CollisionSystem();

	bool collisionAtPoint(int ID, int x, int y);
	bool isColliding(int ID);
	bool hasLineOfSight(int entityID, int otherEntityID);
	bool collisionOnLine(int entityID, Line line);

	bool circleCollision(int ID, int radius);
	bool squareCollision(int ID, int centerX, int centerY, int width, int height);

	CollisionComponent* getCollisionComponent(int ID);
	CollisionComponent* createCollisionComponent(int ID, Shape::ShapeType shape, float centerX, float centerY);

	void updatePosition(int ID, float movedX, float movedY);

	void processMessage(IMessage *message);

	bool rectInsideRect(pRectangle a, pRectangle b);
	bool lineInsideRect(Line line, pRectangle rect);

	bool circleInsideRectangle(pCircle a, pRectangle b);
	bool circleInsideCircle(pCircle a, pCircle b);
	bool lineInsideCircle(Line line, pCircle circle);

	bool pointInsideCircle(pCircle circle, int x, int y);
	bool pointInsideRect(pRectangle rect, int x, int y);

private:
	std::map<int, CollisionComponent*> m_components;
	Grid *m_grid;

	// Checks the specific grid cell for a collision
	bool collisionInCell(int ID, int x, int y);

	// Specific Collision Handling
	bool handleCollision(pShape shapeA, pShape shapeB);
	bool handleCollision(Line line, pShape shape);

	void sendCollisionMessage(int entityID, int collidingEntityID, Vector2D position);
	
	void removeCollisionComponent(int entityID);

	void cleanup();
};

