#include "PhysicsSystem.h"
#include "VelocityIncreaseMessage.h"
#include "MoveMessage.h"
#include "EntityDestroyMessage.h"
#include "IShape.h"
#include <cmath>
#include <iostream>

PhysicsSystem::~PhysicsSystem()
{
	cleanUp();
}

//=============================================================================
// Function: void update(float)
// Description:
// Updates the physics objects.
// Parameters:
// float delta - The time passed since the last update.
//=============================================================================
void PhysicsSystem::update(float delta)
{
	// Try velocity
	applyVelocity(delta);
}

//=============================================================================
// Function: CollisionComponent* createCollisionComponent(int, ShapeType, int, int)
// Description:
// Creates a collision component.
// Parameters:
// int ID - The id of the component to create.
// ShapeType shapeType - The type of shape to create. EX: Rect or Circle.
// int centerX - The X center of the component.
// int centerY - The Y center of the component.
// Output:
// Returns the created collision component on success.
// Returns NULL on failure.
//=============================================================================
CollisionComponent* PhysicsSystem::createCollisionComponent(int ID,
															Shape::ShapeType shapeType,
															int centerX, 
															int centerY)
{
	return m_collisionSystem->createCollisionComponent(ID, shapeType, (float)centerX, (float)centerY);
}

//=============================================================================
// Function: CollisionComponent* getCollisionComponent(int)
// Description:
// Gets the collision component for the ID.
// Parameters:
// int ID - The id of the component to get.
// Output:
// Returns a pointer to the collision component on success.
// Returns NULL on failure.
//=============================================================================
CollisionComponent* PhysicsSystem::getCollisionComponent(int ID)
{
	return m_collisionSystem->getCollisionComponent(ID);
}

//=============================================================================
// Function: VelocityComponent* getVelocityComponent(int)
// Description:
// Gets the velocity component for the ID. If one isn't found, it creates it.
// Parameters:
// int ID - The id of the velocity component to get.
// Output:
// Returns a pointer to the velocity component on success.
// Returns NULL on failure.
//=============================================================================
VelocityComponent* PhysicsSystem::getVelocityComponent(int ID)
{
	VelocityComponent *component = NULL;

	auto mit = m_velocityComponents.find(ID);

	// If the component doesn't exist, create it
	if(mit == m_velocityComponents.end())
	{
		std::cout << "Made a new velocity component!\n";
		component = new VelocityComponent();

		component->setHorizonalSpeed(0);
		component->setVerticalSpeed(0);

		m_velocityComponents.insert(std::make_pair(ID, component));
	}
	// Otherwise set the component to the existing component
	else
	{
		component = mit->second;
	}

	return component;
}


//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes the system message.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void PhysicsSystem::processMessage(IMessage *message)
{
	m_collisionSystem->processMessage(message);

	switch(message->type())
	{
	case IMessage::VELOCITY_INCREASE:
	{
		VelocityIncreaseMessage *velMessage = static_cast<VelocityIncreaseMessage*>(message);
		VelocityComponent *vel = getVelocityComponent(velMessage->m_entityID);

		if(vel)
		{
			float xMax = abs(velMessage->m_xMaxSpeed);
			float yMax = abs(velMessage->m_yMaxSpeed);
			float xIncrease = velMessage->m_xIncrease;
			float yIncrease = velMessage->m_yIncrease;
			float xSpeed = vel->xSpeed();
			float ySpeed = vel->ySpeed();

			float xChange = xSpeed + xIncrease;
			float yChange = ySpeed + yIncrease;

			if(abs(xSpeed) < xMax)
			{
				if(abs(xChange) < xMax)
				{
					vel->increaseHorizontalSpeed(xIncrease);
				}
				else
				{
					float xDiff = abs(xChange) - xMax;
					float tempX = xIncrease;

					if(xIncrease < 0)
					{
						tempX += xDiff;
					}
					else
					{
						tempX -= xDiff;
					}

					vel->increaseHorizontalSpeed(tempX);
				}
			}

			if(abs(ySpeed) < yMax)
			{
				if(abs(yChange) < yMax)
				{
					vel->increaseVerticalSpeed(yIncrease);
				}
				else
				{
					float yDiff = abs(yChange) - yMax;
					float tempY = yIncrease;

					if(yIncrease < 0)
					{
						tempY += yDiff;
					}
					else
					{
						tempY -= yDiff;
					}

					vel->increaseVerticalSpeed(tempY);
				}
			}
		}
		
		break;
	}
	case IMessage::ENTITY_DESTROY:
	{
		EntityDestroyMessage *destroy = static_cast<EntityDestroyMessage*>(message);

		removeVelocityComponent(destroy->m_entityID);

		break;
	}
	}
}

//=============================================================================
// Function: void initCollisionSystem(int, int, int, int, int)
// Description:
// Initializes the collision system and sets up the collision grid.
// Parameters:
// int gridX - The starting x of the grid.
// int gridY - The starting y of the grid.
// int width - The width of the grid.
// int height - The height of the grid.
// int cellSize - The size of the grid cells.
//=============================================================================
void PhysicsSystem::initCollisionSystem(int gridX, int gridY, int width, int height, int cellSize)
{
	if(!m_collisionSystem)
	{
		m_collisionSystem = new CollisionSystem(gridX, gridY, width, height, cellSize);
	}
}

//=============================================================================
// Function: bool hasLineOfSight(int, int)
// Description:
// Checks to see if an entity has line of sight of another entity.
// Parameters:
// int entityID - The entity to see if it has line of sight.
// int otherEntityID - The entity to look for.
// Output:
// Returns true if there is line of sight.
// Returns false if there isn't line of sight.
//=============================================================================
bool PhysicsSystem::hasLineOfSight(int entityID, int otherEntityID)
{
	return m_collisionSystem->hasLineOfSight(entityID, otherEntityID);
}

//=============================================================================
// Function: void sendMoveMessage(int, Vector2D, Vector2D)
// Description:
// Sends a message to the message system, with information about a move that was
// made.
// Parameters:
// int entityID - The ID that was moved.
// Vector2D oldPosition - The position it was moved from.
// Vector2D newPosition - The position it was moved to.
//=============================================================================
void PhysicsSystem::sendMoveMessage(int entityID, Vector2D oldPosition, Vector2D newPosition)
{
	MoveMessage *message = new MoveMessage(entityID, oldPosition, newPosition);

	MessageSystem::instance()->pushMessage(message);
}

//=============================================================================
// Function: void removeVelocityComponent(int)
// Description:
// Finds and deletes the velocity component associated with the ID.
// Parameters:
// int entityID - The ID of the velocity component to delete.
//=============================================================================
void PhysicsSystem::removeVelocityComponent(int entityID)
{
	auto mit = m_velocityComponents.find(entityID);

	if(mit != m_velocityComponents.end())
	{
		delete mit->second;
		mit = m_velocityComponents.erase(mit);
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up all of the physics objects and frees up their memory.
//=============================================================================
void PhysicsSystem::cleanUp()
{
	delete m_collisionSystem;

	auto velMit = m_velocityComponents.begin();

	while (velMit != m_velocityComponents.end())
	{
		delete velMit->second;
		velMit->second = NULL;

		velMit = m_velocityComponents.erase(velMit);

		if (velMit != m_velocityComponents.end())
		{
			velMit++;
		}
	}
}

//=============================================================================
// Function: void applyVelocity(float delta);
// Description:
// Applies the velocity of each velocity component to its corresponding
// collision component.
// Parameters:
// float delta - The time passed since the last update.
//=============================================================================
void PhysicsSystem::applyVelocity(float delta)
{
	// Cycle through the velocity components and apply them to their corresponding 
	// collision objects.

	auto mit = m_velocityComponents.begin();

	while(mit != m_velocityComponents.end())
	{
		VelocityComponent *vel = mit->second;
		int velID = mit->first;

		if (vel)
		{
			if (vel->xSpeed() != 0 || vel->ySpeed() != 0)
			{
				Vector2D start{ 0, 0 };
				Vector2D end{ 0, 0 };

				CollisionComponent *component = m_collisionSystem->getCollisionComponent(velID);
		
				if (component)
				{
					vel->setCurrentVel(vel->velocity());
					start = component->center();
					end = start + (vel->currentVel() * delta);

					float endX = end.getX();
					float endY = end.getY();

					Line line{ start, end };

					// Move the component
					if (!m_collisionSystem->collisionOnLine(mit->first, line))
					{
						m_collisionSystem->updatePosition(mit->first, endX, endY);

						bool moved = false;

						// If we're colliding at the new location
						if (m_collisionSystem->isColliding(mit->first) == true)
						{
							m_collisionSystem->updatePosition(mit->first, start.getX(), end.getY());

							if (m_collisionSystem->isColliding(mit->first))
							{
								m_collisionSystem->updatePosition(mit->first, end.getX(), start.getY());

								if (m_collisionSystem->isColliding(mit->first))
								{
									m_collisionSystem->updatePosition(mit->first, start.getX(), start.getY());
								}
								else
								{
									moved = true;
									sendMoveMessage(mit->first, start, Vector2D(end.getX(), start.getY()));
								}
							}
							else
							{
								moved = true;
								sendMoveMessage(mit->first, start, Vector2D(start.getX(), end.getY()));
							}
						}
						else
						{
							sendMoveMessage(mit->first, start, end);
						}
					}

					applyFriction(mit->first, delta);

				}
			}
		}

		mit++;
	}
}

//=============================================================================
// Function: void applyFriction(int, float)
// Description:
// Applies friction to the velocity.
// TODO: Make it only apply friction to objects opted into friction thx.
// Parameters:
// int entityID - The entity to apply friction for.
// float delta - The time since the last update.
//=============================================================================
void PhysicsSystem::applyFriction(int entityID, float delta)
{
	VelocityComponent *vel = getVelocityComponent(entityID);

	float friction = 10.0f;

	float xSpeed = vel->xSpeed();
	float ySpeed = vel->ySpeed();

	if(abs(xSpeed) < friction)
	{
		vel->increaseHorizontalSpeed(xSpeed * -1);
	}
	else
	{
		if (0 < xSpeed)
		{
			vel->increaseHorizontalSpeed(friction * -1);
		}
		else
		{
			vel->increaseHorizontalSpeed(friction);
		}
	}

	if (abs(ySpeed) < friction)
	{
		vel->increaseVerticalSpeed(ySpeed * -1);
	}
	else
	{
		if (0 < ySpeed)
		{
			vel->increaseVerticalSpeed(friction * -1);
		}
		else
		{
			vel->increaseVerticalSpeed(friction);
		}
	}
}

Vector2D PhysicsSystem::lerp(Vector2D goal, Vector2D current, float amount)
{
	float xDiff = goal.getX() - current.getX();
	float yDiff = goal.getY() - current.getY();
	float tempX = current.getX();
	float tempY = current.getY();

	if(amount < xDiff)
	{
		tempX += amount;
	}
	else if(xDiff < -amount)
	{
		tempX -= amount;
	}
	else
	{
		tempX = goal.getX();
	}

	if(amount < yDiff)
	{
		tempY += amount;
	}
	else if(yDiff < -amount)
	{
		tempY -= amount;
	}
	else
	{
		tempY = goal.getY();
	}

	return Vector2D(tempX, tempY);
}