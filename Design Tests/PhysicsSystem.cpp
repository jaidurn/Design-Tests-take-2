#include "PhysicsSystem.h"
#include "VelocityIncreaseMessage.h"
#include "MoveMessage.h"
#include "IShape.h"
#include <cmath>
#include <iostream>

PhysicsSystem::~PhysicsSystem()
{
	cleanUp();
}

void PhysicsSystem::update()
{
	// Try velocity
	applyVelocity();
}

CollisionComponent* PhysicsSystem::createCollisionComponent(int ID,
															Shape::ShapeType shapeType,
															int centerX, 
															int centerY)
{
	return m_collisionSystem->createCollisionComponent(ID, shapeType, (float)centerX, (float)centerY);
}

CollisionComponent* PhysicsSystem::getCollisionComponent(int ID)
{
	return m_collisionSystem->getCollisionComponent(ID);
}

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
	switch(message->type())
	{
	case IMessage::MessageType::VELOCITY_INCREASE:
	{
		VelocityIncreaseMessage *velMessage = static_cast<VelocityIncreaseMessage*>(message);
		VelocityComponent *vel = getVelocityComponent(velMessage->m_entityID);

		if(vel)
		{
			float xMax = velMessage->m_xMaxSpeed;
			float yMax = velMessage->m_yMaxSpeed;
			float xIncrease = velMessage->m_xIncrease;
			float yIncrease = velMessage->m_yIncrease;
			float xSpeed = vel->xSpeed();
			float ySpeed = vel->ySpeed();

			if (abs(xSpeed + xIncrease) <= abs(xMax))
			{
				vel->increaseHorizontalSpeed(xIncrease);
			}
			else if (abs(xSpeed) < abs(xMax))
			{
				float moveAmount = xMax - xSpeed;

				vel->increaseHorizontalSpeed(moveAmount);
			}

			if(abs(ySpeed + yIncrease) <= abs(yMax))
			{
				vel->increaseVerticalSpeed(yIncrease);
			}
			else if(abs(ySpeed) <= abs(yMax))
			{
				float moveAmount = yMax - ySpeed;

				vel->increaseVerticalSpeed(moveAmount);
			}
		}
	}
	}
}

void PhysicsSystem::initCollisionSystem(int gridX, int gridY, int width, int height, int cellSize)
{
	if(!m_collisionSystem)
	{
		m_collisionSystem = new CollisionSystem(gridX, gridY, width, height, cellSize);
	}
}

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

void PhysicsSystem::applyVelocity()
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
					start = component->center();
					end = start + vel->velocity();

					int endX = (int)round(end.getX());
					int endY = (int)round(end.getY());

					if(endX < 0)
					{
						endX = 0;
						vel->setHorizonalSpeed(vel->xSpeed() * -1);
					}
					else if(1280 < endX)
					{
						endX = 1280;
						vel->setHorizonalSpeed(vel->xSpeed() * -1);
					}

					if(endY < 0)
					{
						endY = 0;
						vel->setVerticalSpeed(vel->ySpeed() * -1);
					}
					else if(720 < endY)
					{
						endY = 720;
						vel->setVerticalSpeed(vel->ySpeed() * -1);
					}

					// Move the component
					m_collisionSystem->updatePosition(mit->first, endX, endY);

					// If we're colliding at the new location
					if (m_collisionSystem->isColliding(mit->first) == true)
					{
						std::cout << "Failure to move!\n";
						// Reset the position
						int startX = (int)round(start.getX());
						int startY = (int)round(start.getY());

						vel->setHorizonalSpeed(vel->xSpeed() * -1);
						vel->setVerticalSpeed(vel->ySpeed() * -1);

						m_collisionSystem->updatePosition(mit->first, startX, startY);
					}
					else
					{
						sendMoveMessage(mit->first, start, end);
						applyFriction(mit->first);
					}

				}
			}
		}

		mit++;
	}
}

//=============================================================================
// Function: void applyFriction()
// Description:
// Applies friction to the velocity.
// TODO: Make it only apply friction to objects opted into friction thx.
// Parameters:
// int entityID - The entity to apply friction for.
//=============================================================================
void PhysicsSystem::applyFriction(int entityID)
{
	VelocityComponent *vel = getVelocityComponent(entityID);

	float friction = 1.0f;

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