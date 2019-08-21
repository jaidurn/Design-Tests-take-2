#include "CollisionSystem.h"
#include "CollisionMessage.h"
#include "EntityDestroyMessage.h"
#include "Vector2D.h"
#include "Rotation.h"
#include "Rectangle.h"
#include "Circle.h"
#include <cmath>

typedef Node<Node<int>*> Cell;

CollisionSystem::CollisionSystem(int gridX, int gridY, int width, int height, int cellSize)
	:m_grid(NULL)
{
	m_grid = new Grid(gridX, gridY, width, height, cellSize);
}


CollisionSystem::~CollisionSystem()
{
	cleanup();
}

bool CollisionSystem::collisionAtPoint(int ID, int x, int y)
{
	bool collision = false;

	CollisionComponent *first = getCollisionComponent(ID);

	if (first)
	{
		Node<int> *node = m_grid->getCoordinateCell(x, y);

		// If the node exists, look through it for possible collisions

		while (node)
		{
			// Make sure we're not checking against the entity requesting
			// the check.
			if (node->data() != ID)
			{
				CollisionComponent *other = getCollisionComponent(node->data());

				pShape otherShape = other->shape();

				switch (otherShape->type())
				{
				case Shape::RECTANGLE:
				{
					pRectangle rect = static_cast<pRectangle>(otherShape);
					collision = (pointInsideRect(rect, x, y) || collision);
				}
				case Shape::CIRCLE:
				{
					pCircle circle = static_cast<pCircle>(otherShape);
					collision = (pointInsideCircle(circle, x, y) || collision);
				}
				}
			}

			node = node->m_next;
		}
	}

	return collision;
}

bool CollisionSystem::isColliding(int ID)
{
	bool collision = false;

	CollisionComponent *first = getCollisionComponent(ID);

	// If the collision component exists
	if(first)
	{
		pShape firstShape = first->shape();

		if(firstShape->type() == Shape::RECTANGLE)
		{
			// Find out what cells the rectangle is located in
			pRectangle rect = static_cast<pRectangle>(firstShape);

			Vector2D topLeft = rect->getTopLeft();
			Vector2D topRight = rect->getTopRight();
			Vector2D bottomLeft = rect->getBottomLeft();
			Vector2D bottomRight = rect->getBottomRight();
			Vector2D center = rect->center();

			// Convert the points to cells
			topLeft = m_grid->convertToCellCoordinates(topLeft);
			topRight = m_grid->convertToCellCoordinates(topRight);
			bottomLeft = m_grid->convertToCellCoordinates(bottomLeft);
			bottomRight = m_grid->convertToCellCoordinates(bottomRight);
			center = m_grid->convertToCellCoordinates(center);

			Vector2D topLeftDist = absoluteValue(topLeft - center);
			Vector2D topRightDist = absoluteValue(topRight - center);
			Vector2D bottomLeftDist = absoluteValue(bottomLeft - center);
			Vector2D bottomRightDist = absoluteValue(bottomRight - center);

			// Find the one with the greatest X distance
			float tempX = topLeftDist.getX();

			if (tempX < topRightDist.getX()) { tempX = topRightDist.getX(); }
			if (tempX < bottomLeftDist.getX()) { tempX = bottomLeftDist.getX(); }
			if (tempX < bottomRightDist.getX()) { tempX = bottomRightDist.getX(); }

			float tempY = topLeftDist.getY();
			
			if (tempY < topRightDist.getY()) { tempY = topRightDist.getY(); }
			if (tempY < bottomLeftDist.getY()) { tempY = bottomLeftDist.getY(); }
			if (tempY < bottomRightDist.getY()) { tempY = bottomRightDist.getY(); }

			if (tempX <= 0) { tempX = 1; }
			if (tempY <= 0) { tempY = 1; }

			int distanceX = (int)round(tempX);
			int distanceY = (int)round(tempY);

			// Find the starting grid space
			int startingX = (int)round(center.getX() - tempX);
			int startingY = (int)round(center.getY() - tempY);

			int endingX = (int)round(center.getX() + tempX);
			int endingY = (int)round(center.getY() + tempY);

			// If starting position is off the grid, fix everything
			if (startingX < 0) 
			{ 
				startingX = 0;
			}
			if (startingY < 0) 
			{ 
				startingY = 0;
			}

			for(int y = startingY; y <= endingY; y++)
			{
				if (y < m_grid->rowCount())
				{
					for (int x = startingX; x <= endingX; x++)
					{
						if (x < m_grid->columnCount())
						{
							collision = collisionInCell(ID, x, y) || collision;
						}
						// We can't check anymore X, so break.
						else { break; }
					}
				}
				// We're unable to check any more, so exit the check.
				else { break; }
			}

	}
		else if(firstShape->type() == Shape::CIRCLE)
		{
			pCircle circle = static_cast<pCircle>(firstShape);

			// Find the farthest point away
			float radius = (float)circle->radius();
			
			Vector2D center = circle->center();
			Vector2D topPoint{ center.getX(), center.getY() - radius };
			Vector2D bottomPoint{ center.getX(), center.getY() + radius };
			Vector2D leftPoint{ center.getX() - radius, center.getY() };
			Vector2D rightPoint{ center.getX() + radius, center.getY() };

			// Convert those points to the grid
			center = m_grid->convertToCellCoordinates(center);
			topPoint = m_grid->convertToCellCoordinates(topPoint);
			bottomPoint = m_grid->convertToCellCoordinates(bottomPoint);
			leftPoint = m_grid->convertToCellCoordinates(leftPoint);
			rightPoint = m_grid->convertToCellCoordinates(rightPoint);

			Vector2D topPointDist = absoluteValue(topPoint - center);
			Vector2D bottomPointDist = absoluteValue(bottomPoint - center);
			Vector2D leftPointDist = absoluteValue(leftPoint - center);
			Vector2D rightPointDist = absoluteValue(rightPoint - center);

			float tempX = leftPointDist.getX();
			// We only have to check the left and right for X
			if (tempX < rightPointDist.getX()) { tempX = rightPointDist.getX(); }

			float tempY = topPointDist.getY();
			// We only have to check the top and bottom for Y
			if (tempY < bottomPointDist.getY()) { tempY = bottomPointDist.getY(); }

			// If we're only checking one area for some reason, check more!
			if (tempX <= 0) { tempX = 1; }
			if (tempY <= 0) { tempY = 1; }

			int distanceX = (int)round(tempX);
			int distanceY = (int)round(tempY);

			int totalXDistance = distanceX * 2;
			int totalYDistance = distanceY * 2;

			int startingX = (int)round(center.getX() - tempX);
			int startingY = (int)round(center.getY() - tempY);

			// If starting position is off the grid, fix everything
			if (startingX < 0)
			{
				totalXDistance -= startingX;
				startingX = 0;
			}
			if (startingY < 0)
			{
				totalYDistance -= startingY;
				startingY = 0;
			}

			for(int y = startingY; y < totalYDistance; y++)
			{
				if (y < m_grid->rowCount())
				{
					for (int x = startingX; x < totalXDistance; x++)
					{
						if (x < m_grid->columnCount())
						{
							collision = collisionInCell(ID, x, y) || collision;
						}
						// We can't check anymore X, so break.
						else { break; }
					}
				}
				// We're unable to check any more, so exit the check.
				else { break; }
			}
		}
	}

	return collision;
}

//=============================================================================
// Function: bool hasLineOfSight(int, int)
// Description:
// Checks to see if there's an unblocked line of sight between two entities.
// Parameters:
// int entityID - The first entity to start with.
// int otherEntityID - The entity to check for line of sight.
// Output:
// bool - Returns true if there is line of sight.
// Returns false if unsuccessful.
//=============================================================================
bool CollisionSystem::hasLineOfSight(int entityID, int otherEntityID)
{
	bool hasSight = false;

	CollisionComponent *first = getCollisionComponent(entityID);
	CollisionComponent *second = getCollisionComponent(otherEntityID);

	if (first && second)
	{
		hasSight = true;

		// Find the top and bottom line of sight
		Vector2D top{ 0.0f, 0.0f };
		Vector2D bottom{ 0.0f, 0.0f };

		if (second->shape()->type() == Shape::RECTANGLE)
		{
			pRectangle rect = static_cast<pRectangle>(second->shape());

			bool onLeftSide = false;

			if (rect->center().getX() < first->center().getX())
			{
				onLeftSide = true;
			}


			// If the rectangle is rotated, we have to find the right points
			// to look for.
			if (rect->rotation() != 0.0f)
			{
				// Find the two points with the furthest distance on the
				// Y axis from the center.
				Vector2D center = rect->center();

				Vector2D topLeft = rect->getTopLeft();
				Vector2D topRight = rect->getTopRight();
				Vector2D bottomLeft = rect->getBottomLeft();
				Vector2D bottomRight = rect->getBottomRight();

				Vector2D currentTop = topLeft;
				Vector2D currentBottom = bottomRight;

				float tl_yDist = topLeft.getY() - center.getY();
				float tr_yDist = topRight.getY() - center.getY();

				if (abs(tl_yDist) < abs(tr_yDist))
				{
					currentTop = topRight;
					currentBottom = bottomLeft;
				}
				else if (abs(tl_yDist) == abs(tr_yDist))
				{
					if (topRight.getY() < topLeft.getY() && onLeftSide)
					{
						currentTop = bottomRight;
						currentBottom = bottomLeft;
					}
					else if (topLeft.getY() < topRight.getY() && !onLeftSide)
					{
						currentTop = bottomLeft;
						currentBottom = bottomRight;
					}
				}

				top = currentTop;
				bottom = currentBottom;
			}
			else
			{
				if (onLeftSide)
				{
					top = rect->getTopRight();
					bottom = rect->getBottomRight();
				}
				else
				{
					top = rect->getTopLeft();
					bottom = rect->getBottomLeft();
				}
			}
		}
		else if (second->shape()->type() == Shape::CIRCLE)
		{
			pCircle circle = static_cast<pCircle>(second->shape());

			top.setX(circle->center().getX());
			top.setY(circle->center().getY() - (float)circle->radius());

			bottom.setX(circle->center().getX());
			bottom.setY(circle->center().getY() - (float)circle->radius());
		}

		Line lineToTop{ first->center(), top };
		Line lineToBottom{ first->center(), bottom };

		Line lineOfSight{ first->center(), second->center() };

		Vector2D gridStart = lineOfSight.start;
		Vector2D gridEnd = lineOfSight.end;

		gridStart = m_grid->convertToCellCoordinates(gridStart);
		gridEnd = m_grid->convertToCellCoordinates(gridEnd);

		int xStart = (int)round(gridStart.getX());
		int yStart = (int)round(gridStart.getY());
		int xEnd = (int)round(gridEnd.getX());
		int yEnd = (int)round(gridEnd.getY());

		bool topSeen = true;
		bool bottomSeen = true;
		bool centerSeen = true;

		if (gridEnd.getX() < gridStart.getX())
		{
			xStart = xEnd;
			xEnd = (int)round(gridStart.getX());
		}

		if (gridEnd.getY() < gridStart.getY())
		{
			yStart = yEnd;
			yEnd = (int)round(gridStart.getY());
		}

		if (xStart == xEnd || yStart == yEnd)
		{
			if (xStart == xEnd && yStart != yEnd)
			{
				for (int i = yStart; i < yEnd; i++)
				{
					Node<int> *cell = m_grid->getCell(xStart, i);

					while (cell != NULL)
					{
						if (cell->data() != entityID && cell->data() != otherEntityID)
						{
							CollisionComponent *comp = getCollisionComponent(cell->data());

							if (comp)
							{
								if (handleCollision(lineOfSight, comp->shape()))
								{
									std::cout << "Collided with entity: " << cell->data() << std::endl;
									hasSight = false;
								}
							}
						}

						cell = cell->m_next;
					}
				}
			}
			else if (yStart == yEnd && xStart != xEnd)
			{
				for (int i = xStart; i < xEnd; i++)
				{
					Node<int> *cell = m_grid->getCell(i, yStart);

					while (cell != NULL)
					{
						if (cell->data() != entityID && cell->data() != otherEntityID)
						{
							CollisionComponent *comp = getCollisionComponent(cell->data());

							if (comp)
							{
								if (handleCollision(lineOfSight, comp->shape()))
								{
									centerSeen = false;
								}

								if (handleCollision(lineToTop, comp->shape()))
								{
									topSeen = false;
								}

								if (handleCollision(lineToBottom, comp->shape()))
								{
									bottomSeen = false;
								}
							}
						}

						cell = cell->m_next;
					}
				}
			}
			else
			{
				Node<int> *cell = m_grid->getCell(xStart, yStart);

				while (cell != NULL)
				{
					if (cell->data() != entityID && cell->data() != otherEntityID)
					{
						CollisionComponent *comp = getCollisionComponent(cell->data());

						if (comp)
						{
							if (handleCollision(lineOfSight, comp->shape()))
							{
								centerSeen = false;
							}

							if (handleCollision(lineToTop, comp->shape()))
							{
								topSeen = false;
							}

							if (handleCollision(lineToBottom, comp->shape()))
							{
								bottomSeen = false;
							}
						}
					}

					cell = cell->m_next;
				}
			}
		}
		else
		{
			for (int i = xStart; i < xEnd; i++)
			{
				if (0 <= i && i < m_grid->columnCount())
				{
					for (int j = yStart; j < yEnd; j++)
					{
						if (0 <= j && j < m_grid->rowCount())
						{
							Node<int> *cell = m_grid->getCell(i, j);

							while (cell != NULL)
							{
								if (cell->data() != entityID && cell->data() != otherEntityID)
								{
									CollisionComponent *comp = getCollisionComponent(cell->data());

									if (comp)
									{
										if (handleCollision(lineOfSight, comp->shape()))
										{
											centerSeen = false;
										}

										if (handleCollision(lineToTop, comp->shape()))
										{
											topSeen = false;
										}

										if (handleCollision(lineToBottom, comp->shape()))
										{
											bottomSeen = false;
										}
									}
								}

								cell = cell->m_next;
							}
						}
					}
				}
			}
		}

		hasSight = (centerSeen || topSeen || bottomSeen);
	}

	return hasSight;
}

bool CollisionSystem::collisionOnLine(int entityID, Line line)
{
	bool collision = false;

	Vector2D gridStart = line.start;
	Vector2D gridEnd = line.end;

	int gridMaxX = m_grid->cellSize() * m_grid->columnCount();
	int gridMaxY = m_grid->cellSize() * m_grid->rowCount();

	if (gridStart.getX() < 0) { gridStart.setX(0); }
	else if (gridMaxY < gridStart.getX()) { gridStart.setX((float)gridMaxY); }

	if (gridStart.getY() < 0) { gridStart.setY(0); }
	else if (gridMaxY < gridStart.getY()) { gridStart.setY((float)gridMaxY); }
	
	if (gridEnd.getX() < 0) { gridEnd.setX(0); }
	else if (gridMaxX < gridEnd.getX()) { gridEnd.setX((float)gridMaxX); }
	
	if (gridEnd.getY() < 0) { gridEnd.setY(0); }
	else if (gridMaxY < gridEnd.getY()) { gridEnd.setY((float)gridMaxY); }

	gridStart = m_grid->convertToCellCoordinates(gridStart);
	gridEnd = m_grid->convertToCellCoordinates(gridEnd);

	int xStart = (int)round(gridStart.getX());
	int yStart = (int)round(gridStart.getY());
	int xEnd = (int)round(gridEnd.getX());
	int yEnd = (int)round(gridEnd.getY());

	if (gridEnd.getX() < gridStart.getX())
	{
		xStart = xEnd;
		xEnd = (int)round(gridStart.getX());
	}

	if (gridEnd.getY() < gridStart.getY())
	{
		yStart = yEnd;
		yEnd = (int)round(gridStart.getY());
	}

	if (xStart == xEnd || yStart == yEnd)
	{
		if (xStart == xEnd && yStart != yEnd)
		{
			for (int i = yStart; i < yEnd; i++)
			{
				Node<int> *cell = m_grid->getCell(xStart, i);

				while (cell != NULL)
				{
					if (cell->data() != entityID)
					{
						CollisionComponent *comp = getCollisionComponent(cell->data());

						if (comp)
						{
							if (handleCollision(line, comp->shape()))
							{
								if (comp->isSolid())
								{
									collision = true;
								}
							}
						}
					}

					cell = cell->m_next;
				}
			}
		}
		else if (yStart == yEnd && xStart != xEnd)
		{
			for (int i = xStart; i < xEnd; i++)
			{
				Node<int> *cell = m_grid->getCell(i, yStart);

				while (cell != NULL)
				{
					if (cell->data() != entityID)
					{
						CollisionComponent *comp = getCollisionComponent(cell->data());

						if (comp)
						{
							if (handleCollision(line, comp->shape()))
							{
								if (comp->isSolid())
								{
									collision = true;
								}
							}
						}
					}

					cell = cell->m_next;
				}
			}
		}
		else
		{
			Node<int> *cell = m_grid->getCell(xStart, yStart);

			while (cell != NULL)
			{
				if (cell->data() != entityID)
				{
					CollisionComponent *comp = getCollisionComponent(cell->data());

					if (comp)
					{
						if (handleCollision(line, comp->shape()))
						{
							if (comp->isSolid())
							{
								collision = true;
							}
						}
					}
				}

				cell = cell->m_next;
			}
		}
	}
	else
	{
		for (int i = xStart; i < xEnd; i++)
		{
			if (0 <= i && i < m_grid->columnCount())
			{
				for (int j = yStart; j < yEnd; j++)
				{
					if (0 <= j && j < m_grid->rowCount())
					{
						Node<int> *cell = m_grid->getCell(i, j);

						while (cell != NULL)
						{
							if (cell->data() != entityID)
							{
								CollisionComponent *comp = getCollisionComponent(cell->data());

								if (comp)
								{
									if (handleCollision(line, comp->shape()))
									{
										if (comp->isSolid())
										{
											collision = true;
										}
									}
								}
							}

							cell = cell->m_next;
						}
					}
				}
			}
		}
	}

	return collision;
}

bool CollisionSystem::circleCollision(int ID, int radius)
{
	bool isColliding = false;

	CollisionComponent *collision = getCollisionComponent(ID);

	if (collision)
	{
		Shape::Circle circle(collision->center().getX(), collision->center().getY(), radius);

		// Find the farthest point away
		float radius = (float)circle.radius();

		Vector2D center = circle.center();
		Vector2D topPoint{ center.getX(), center.getY() - radius };
		Vector2D bottomPoint{ center.getX(), center.getY() + radius };
		Vector2D leftPoint{ center.getX() - radius, center.getY() };
		Vector2D rightPoint{ center.getX() + radius, center.getY() };

		// Convert those points to the grid
		center = m_grid->convertToCellCoordinates(center);
		topPoint = m_grid->convertToCellCoordinates(topPoint);
		bottomPoint = m_grid->convertToCellCoordinates(bottomPoint);
		leftPoint = m_grid->convertToCellCoordinates(leftPoint);
		rightPoint = m_grid->convertToCellCoordinates(rightPoint);

		Vector2D topPointDist = absoluteValue(topPoint - center);
		Vector2D bottomPointDist = absoluteValue(bottomPoint - center);
		Vector2D leftPointDist = absoluteValue(leftPoint - center);
		Vector2D rightPointDist = absoluteValue(rightPoint - center);

		float tempX = leftPointDist.getX();
		// We only have to check the left and right for X
		if (tempX < rightPointDist.getX()) { tempX = rightPointDist.getX(); }

		float tempY = topPointDist.getY();
		// We only have to check the top and bottom for Y
		if (tempY < bottomPointDist.getY()) { tempY = bottomPointDist.getY(); }

		// If we're only checking one area for some reason, check more!
		if (tempX <= 0) { tempX = 1; }
		if (tempY <= 0) { tempY = 1; }

		int distanceX = (int)round(tempX);
		int distanceY = (int)round(tempY);

		int totalXDistance = distanceX * 2;
		int totalYDistance = distanceY * 2;

		int startingX = (int)round(center.getX() - tempX);
		int startingY = (int)round(center.getY() - tempY);

		// If starting position is off the grid, fix everything
		if (startingX < 0)
		{
			totalXDistance -= startingX;
			startingX = 0;
		}
		if (startingY < 0)
		{
			totalYDistance -= startingY;
			startingY = 0;
		}

		for (int y = startingY; y < totalYDistance; y++)
		{
			if (y < m_grid->rowCount())
			{
				for (int x = startingX; x < totalXDistance; x++)
				{
					if (x < m_grid->rowCount())
					{
						Node<int> *cell = m_grid->getCell(x, y);

						while(cell != NULL)
						{
							if(cell->data() != ID)
							{
								pShape shapeA = static_cast<pShape>(&circle);
								CollisionComponent *componentB = getCollisionComponent(cell->data());

								if (componentB)
								{
									if (handleCollision(shapeA, componentB->shape()))
									{
										isColliding = true;

										sendCollisionMessage(ID, cell->data(), circle.center());
									}
								}
							}

							cell = cell->m_next;
						}
					}
					// We can't check anymore X, so break.
					else { break; }
				}
			}
			// We're unable to check any more, so exit the check.
			else { break; }
		}

	}

	return isColliding;
}

bool CollisionSystem::squareCollision(int ID, int centerX, int centerY, int width, int height)
{
	bool colliding = false;

	// Find out what cells the rectangle is located in
	Shape::Rectangle rect((float)centerX, (float)centerY, width, height);

	Vector2D topLeft = rect.getTopLeft();
	Vector2D topRight = rect.getTopRight();
	Vector2D bottomLeft = rect.getBottomLeft();
	Vector2D bottomRight = rect.getBottomRight();
	Vector2D center = rect.center();

	// Convert the points to cells
	topLeft = m_grid->convertToCellCoordinates(topLeft);
	topRight = m_grid->convertToCellCoordinates(topRight);
	bottomLeft = m_grid->convertToCellCoordinates(bottomLeft);
	bottomRight = m_grid->convertToCellCoordinates(bottomRight);
	center = m_grid->convertToCellCoordinates(center);

	Vector2D topLeftDist = absoluteValue(topLeft - center);
	Vector2D topRightDist = absoluteValue(topRight - center);
	Vector2D bottomLeftDist = absoluteValue(bottomLeft - center);
	Vector2D bottomRightDist = absoluteValue(bottomRight - center);

	// Find the one with the greatest X distance
	float tempX = topLeftDist.getX();

	if (tempX < topRightDist.getX()) { tempX = topRightDist.getX(); }
	if (tempX < bottomLeftDist.getX()) { tempX = bottomLeftDist.getX(); }
	if (tempX < bottomRightDist.getX()) { tempX = bottomRightDist.getX(); }

	float tempY = topLeftDist.getY();

	if (tempY < topRightDist.getY()) { tempY = topRightDist.getY(); }
	if (tempY < bottomLeftDist.getY()) { tempY = bottomLeftDist.getY(); }
	if (tempY < bottomRightDist.getY()) { tempY = bottomRightDist.getY(); }

	if (tempX <= 0) { tempX = 1; }
	if (tempY <= 0) { tempY = 1; }

	int distanceX = (int)round(tempX);
	int distanceY = (int)round(tempY);

	int totalXDistance = distanceX * 2;
	int totalYDistance = distanceY * 2;

	// Find the starting grid space
	int startingX = (int)round(center.getX() - tempX);
	int startingY = (int)round(center.getY() - tempY);

	// If starting position is off the grid, fix everything
	if (startingX < 0)
	{
		totalXDistance -= startingX;
		startingX = 0;
	}
	if (startingY < 0)
	{
		totalYDistance -= startingY;
		startingY = 0;
	}

	for (int y = startingY; y < totalYDistance; y++)
	{
		if (y < m_grid->rowCount())
		{
			for (int x = startingX; x < totalXDistance; x++)
			{
				if (x < m_grid->columnCount())
				{
					Node<int> *cell = m_grid->getCell(x, y);

					while (cell != NULL)
					{
						if (cell->data() != ID)
						{
							pShape shapeA = static_cast<pShape>(&rect);
							CollisionComponent *componentB = getCollisionComponent(cell->data());

							if (componentB)
							{
								if (handleCollision(shapeA, componentB->shape()))
								{
									colliding = true;

									sendCollisionMessage(ID, cell->data(), Vector2D((float)centerX, (float)centerY));
								}
							}
						}

						cell = cell->m_next;
					}
				}
				// We can't check anymore X, so break.
				else { break; }
			}
		}
		// We're unable to check any more, so exit the check.
		else { break; }
	}

	return colliding;
}

CollisionComponent* CollisionSystem::getCollisionComponent(int ID)
{
	CollisionComponent *component = NULL;

	auto mit = m_components.find(ID);

	if (mit != m_components.end())
	{
		component = mit->second;
	}

	return component;
}

CollisionComponent* CollisionSystem::createCollisionComponent(
	int ID,
	Shape::ShapeType shape,
	float centerX,
	float centerY)
{
	CollisionComponent *component = NULL;

	auto mit = m_components.find(ID);

	// If the component doesn't already exist create it
	if (mit == m_components.end())
	{
		pShape shapeToCreate = NULL;

		// Decide which shape to create
		switch (shape)
		{
		case Shape::RECTANGLE:
		{
			shapeToCreate = new Shape::Rectangle();
			break;
		}
		case Shape::CIRCLE:
		{
			shapeToCreate = new Shape::Circle();
			break;
		}
		}

		// Make sure the shape was properly created before continuing
		if (shapeToCreate)
		{
			shapeToCreate->setCenter(centerX, centerY);

			component = new CollisionComponent(shapeToCreate);

			// Add the object to the grid
			int gridX = (int)(round(centerX));
			int gridY = (int)(round(centerY));

//			std::cout << "adding object to grid!\n";
			m_grid->add(ID, gridX, gridY);

			m_components.insert(std::make_pair(ID, component));
		}
	}
	// Otherwise, return the existing collision component
	else
	{
		component = mit->second;
	}

	return component;
}

void CollisionSystem::updatePosition(int ID, float movedX, float movedY)
{
	CollisionComponent *component = getCollisionComponent(ID);
	
	// Make sure there's a component to move before moving it
	if (component)
	{
		pShape shape = component->shape();
		Vector2D shapeCenter{ shape->center() };

		int oldX = (int)(round(shapeCenter.getX()));
		int oldY = (int)(round(shapeCenter.getY()));

		// Move its location on the grid
		m_grid->move(ID, oldX, oldY, (int)round(movedX), (int)round(movedY));

		// Actually move it
		shape->setCenter(movedX, movedY);
	}
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes the messages passed to it.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void CollisionSystem::processMessage(IMessage *message)
{
	switch(message->type())
	{
	case IMessage::ENTITY_DESTROY:
	{
		EntityDestroyMessage *destroy = static_cast<EntityDestroyMessage*>(message);

		removeCollisionComponent(destroy->m_entityID);

		break;
	}
	}
}

// Private Functions
bool CollisionSystem::collisionInCell(int ID, int x, int y)
{
	bool collision = false;

	Node<int> *node = m_grid->getCell(x, y);

	while(node != NULL)
	{
		if(node->data() != ID)
		{
			// Check for a collision
			CollisionComponent *a = getCollisionComponent(ID);
			CollisionComponent *b = getCollisionComponent(node->data());

			if (b)
			{
				if (handleCollision(a->shape(), b->shape()) == true)
				{
					//Get the center point
					Vector2D centerPoint(a->center());

					// TODO: Limit message sending. Don't send for every collision.
					sendCollisionMessage(ID, node->data(), centerPoint);

					if (b->isSolid())
					{
						collision = true;
						return collision;
					}
				}
			}
			else
			{
				collision = false;
			}
		}

		node = node->m_next;
	}

	return collision;
}

bool CollisionSystem::handleCollision(pShape shapeA, pShape shapeB)
{
	bool collision = true;

	if(shapeA->type() == Shape::RECTANGLE)
	{
		pRectangle rectA = static_cast<pRectangle>(shapeA);

		if(shapeB->type() == Shape::CIRCLE)
		{
			pCircle circleB = static_cast<pCircle>(shapeB);

			collision = circleInsideRectangle(circleB, rectA);
		}
		else if(shapeB->type() == Shape::RECTANGLE)
		{
			pRectangle rectB = static_cast<pRectangle>(shapeB);

			collision = rectInsideRect(rectA, rectB);
		}

	}
	else if(shapeA->type() == Shape::CIRCLE)
	{
		pCircle circleA = static_cast<pCircle>(shapeA);

		if (shapeB->type() == Shape::CIRCLE)
		{
			pCircle circleB = static_cast<pCircle>(shapeB);

			collision = circleInsideCircle(circleA, circleB);
		}
		else if (shapeB->type() == Shape::RECTANGLE)
		{
			pRectangle rectB = static_cast<pRectangle>(shapeB);

			collision = circleInsideRectangle(circleA, rectB);
		}
	}

	return collision;
}

//=============================================================================
// Function: bool handleCollision(Line, pShape)
// Description:
// Handles a collision between a line and a collision shape.
// Parameters:
// Line line - The line to check.
// pShape - The shape to check.
// Output:
// bool - Returns true on collision.
// Returns false on no collision.
//=============================================================================
bool CollisionSystem::handleCollision(Line line, pShape shape)
{
	bool collision = false;

	if(shape)
	{
		switch(shape->type())
		{
		case Shape::CIRCLE:
		{
			pCircle circle = static_cast<pCircle>(shape);

			collision = lineInsideCircle(line, circle);
			break;
		}
		case Shape::RECTANGLE:
		{
			pRectangle rect = static_cast<pRectangle>(shape);

			collision = lineInsideRect(line, rect);

			break;
		}
		}
	}

	return collision;
}

bool CollisionSystem::circleInsideCircle(pCircle a, pCircle b)
{
	bool inside = false;

	int x1, x2, y1, y2;

	Vector2D centerA{ a->center() };
	Vector2D centerB{ b->center() };

	x1 = (int)round(centerA.getX());
	y1 = (int)round(centerA.getY());

	x2 = (int)round(centerB.getX());
	y2 = (int)round(centerB.getY());

	int xDistance = (x2 - x1);
	int yDistance = (y2 - y1);

	int totalDistance = (int)sqrt((xDistance * xDistance) + (yDistance * yDistance));

	if (totalDistance < (a->radius() + b->radius()))
	{
		inside = true;
	}

	return inside;
}

bool CollisionSystem::circleInsideRectangle(pCircle a, pRectangle b)
{
	bool inside = true;

	int circleTop, circleBottom, circleLeft, circleRight;
	int rectTop, rectBottom, rectLeft, rectRight;

	Vector2D centerA{ a->center() };
	Vector2D centerB{ b->center() };

	// Rotate the circle around the point
	centerA = rotatePoint(centerA, centerB, b->rotation() * -1);

	int cX = (int)round(centerA.getX());
	int cY = (int)round(centerA.getY());

	int rectLeftX = (int)round(centerB.getX() - (b->width() / 2));
	int rectRightX = (int)round(centerB.getX() + (b->width() / 2));
	int rectTopY = (int)round(centerB.getY() - (b->height() / 2));
	int rectBottomY = (int)round(centerB.getY() + (b->height() / 2));

	int closestX = 0;
	int closestY = 0;

	//Find the closest X point
	if (cX < rectLeftX) { closestX = rectLeftX; }
	else if (rectRightX < cX) { closestX = rectRightX; }
	else { closestX = cX; }

	//Find the closest Y point
	if (cY < rectTopY) { closestY = rectTopY; }
	else if (rectBottomY < cY) { closestY = rectBottomY; }
	else { closestY = cY; }

	// Check the distance between the circle and the closest point
	float xDistance = centerA.getX() - closestX;
	float yDistance = centerA.getY() - closestY;

	xDistance *= xDistance;
	yDistance *= yDistance;

	float totalDistance = xDistance + yDistance;

	totalDistance = (float)sqrt(totalDistance);

	if (a->radius() < totalDistance)
	{
		inside = false;
	}

	return inside;
}

bool CollisionSystem::rectInsideRect(pRectangle a, pRectangle b)
{
	bool inside = true;

	if (a->rotation() == 0 && b->rotation() == 0)
	{
		Vector2D aTopLeft = a->getTopLeft();
		Vector2D aBottomRight = a->getBottomRight();
		Vector2D bTopLeft = b->getTopLeft();
		Vector2D bBottomRight = b->getBottomRight();

		if(bBottomRight.getX() <= aTopLeft.getX())
		{
			inside = false;
		}
		else if(aBottomRight.getX() <= bTopLeft.getX())
		{
			inside = false;
		}
		else if(bBottomRight.getY() <= aTopLeft.getY())
		{
			inside = false;
		}
		else if(aBottomRight.getY() <= bTopLeft.getY())
		{
			inside = false;
		}
	}
	else
	{
		inside = false;

		// Set up all of the corners with their rotation
		Vector2D centerA{ a->center() };
		Vector2D topLeftA = a->getTopLeft();
		Vector2D topRightA = a->getTopRight();
		Vector2D bottomLeftA = a->getBottomLeft();
		Vector2D bottomRightA = a->getBottomRight();

		Vector2D centerB{ b->center() };
		Vector2D topLeftB = b->getTopLeft();
		Vector2D topRightB = b->getTopRight();
		Vector2D bottomLeftB = b->getBottomLeft();
		Vector2D bottomRightB = b->getBottomRight();

		// Check the distance between all the points and the center
		// of the other rectangle's center.
		const int RECT_POINTS = 4;

		Vector2D pointsA[RECT_POINTS]{ topLeftA, topRightA, bottomRightA, bottomLeftA };
		Vector2D pointsB[RECT_POINTS]{ topLeftB, topRightB, bottomRightB, bottomLeftB };
	
		float aDistances[RECT_POINTS];
		float bDistances[RECT_POINTS];

		for(int i = 0; i < RECT_POINTS; i++)
		{
			aDistances[i] = totalDistance(pointsA[i], centerB);
			bDistances[i] = totalDistance(pointsB[i], centerA);
		}

		float smallestDistanceA = aDistances[0];
		float smallestDistanceB = bDistances[0];
		int indexA = 0;
		int indexB = 0;

		// Find the smallest distance for both rects
		for(int i = 0; i < RECT_POINTS; i++)
		{
			if(aDistances[i] < smallestDistanceA)
			{
				smallestDistanceA = aDistances[i];
				indexA = i;
			}

			if(bDistances[i] < smallestDistanceB)
			{
				smallestDistanceB = bDistances[i];
				indexB = i;
			}
		}

		// Determine which points to use for checking
		int prevPointA = indexA - 1;
		int nextPointA = indexA + 1;
		
		// Make sure the points are within range
		if (indexA == 0) { prevPointA = RECT_POINTS - 1; }
		if (indexA == RECT_POINTS - 1) { nextPointA = 0; }
		
		// Do the same for B
		int prevPointB = indexB - 1;
		int nextPointB = indexB + 1;

		if (indexB == 0) { prevPointB = RECT_POINTS - 1; }
		if (indexB == RECT_POINTS - 1) { nextPointB = 0; }

		Line lineA1{ pointsA[indexA], pointsA[prevPointA] };
		Line lineA2{ pointsA[indexA], pointsA[nextPointA] };
		Line lineB1{ pointsB[indexB], pointsB[prevPointB] };
		Line lineB2{ pointsB[indexB], pointsB[nextPointB] };

		if(linesIntersect(lineA1, lineB1) || linesIntersect(lineA1, lineB2) ||
		   linesIntersect(lineA2, lineB1) || linesIntersect(lineA2, lineB2))
		{
			// Figure out why the collision is firing from so far away.
			// Also clean up this function with my new functionality
			inside = true;
		}
	}

	return inside;
}

bool CollisionSystem::pointInsideCircle(pCircle circle, int x, int y)
{
	bool inside = false;

	Vector2D circleCenter{ circle->center() };

	int xDistance = (int)round(x - circleCenter.getX());
	int yDistance = (int)round(y - circleCenter.getY());
	int radius = circle->radius();

	// Remember, A^2 + B^2 = C^2
	int totalDistance = (xDistance * xDistance) + (yDistance * yDistance);

	if (totalDistance < (radius * radius))
	{
		inside = true;
	}

	return inside;
}

bool CollisionSystem::pointInsideRect(pRectangle rect, int x, int y)
{
	bool inside = true;

	Vector2D rectCenter{ rect->center() };
	Vector2D point((float)x, (float)y);

	// Rotate the point to normalize it around the rect
	point = rotatePoint(point, rectCenter, rect->rotation() * -1);

	int leftX =(int)round(rectCenter.getX() - (rect->width() / 2));
	int topY = (int)round(rectCenter.getY() - (rect->height() / 2));
	int rightX = (int)round(rectCenter.getX() + (rect->width() / 2));
	int bottomY = (int)round(rectCenter.getY() + (rect->height() / 2));

	// If the x is to the left or right of the rect, it's not inside
	if(x < leftX)
	{
		inside = false;
	}
	else if (rightX < x)
	{
		inside = false;
	}
	
	// If the y is above or below the rect, it's not inside
	if(y < topY)
	{
		inside = false;
	}
	else if(bottomY < y)
	{
		inside = false;
	}

	return inside;
}

//=============================================================================
// Function: bool lineInsideCircle(Line, pCircle)
// Description:
// Checks to see if a line intersects a circle.
// Parameters:
// Line line - The line to check.
// pCircle circle - The circle to check.
// Output:
// bool - Returns true on collision.
// Returns false on no collision.
//=============================================================================
bool CollisionSystem::lineInsideCircle(Line line, pCircle circle)
{
	bool collision = false;

	if(circle)
	{
		int Ax = (int)round(line.start.getX());
		int Ay = (int)round(line.start.getY());
		int Bx = (int)round(line.end.getX());
		int By = (int)round(line.end.getY());

		int xDist = (Bx - Ax) * (Bx - Ax);
		int yDist = (By - Ay) * (By - Ay);

		double totalDist = sqrt((xDist + yDist));

		int circleX = (int)round(circle->center().getX());
		int circleY = (int)round(circle->center().getY());
		int radius = (int)round(circle->radius());

		double Dx = ((double)Bx - (double)Ax) / totalDist;
		double Dy = ((double)By - (double)Ay) / totalDist;
		double t = Dx * ((double)circleX - (double)Ax) + Dy * ((double)circleY - (double)Ay);

		double Ex = t * Dx + Ax;
		double Ey = t * Dy + Ay;

		double distanceExCx = (Ex - circleX) * (Ex - circleX);
		double distanceEyCy = (Ey - circleY) * (Ey - circleY);

		double totalDistanceEC = sqrt(distanceExCx + distanceEyCy);

		if(totalDistanceEC < radius)
		{
			collision = true;
		}
	}

	return collision;
}

//=============================================================================
// Function: bool lineInsideRect(Line, pRectangle)
// Description:
// Checks to see if a line intersects a rectangle.
// Parameters:
// Line line - The line to check.
// pRectangle rect - The rectangle to check.
// Output:
// bool - Returns true on collision.
// Returns false if there's no collision.
//=============================================================================
bool CollisionSystem::lineInsideRect(Line line, pRectangle rect)
{
	bool collision = false;

	if (rect)
	{
		// Create lines out of all four of the rectangle's sides.
		Line left{ rect->getTopLeft(), rect->getBottomLeft() };
		Line right{ rect->getTopRight(), rect->getBottomRight() };
		Line top{ rect->getTopLeft(), rect->getTopRight() };
		Line bottom{ rect->getBottomLeft(), rect->getBottomRight() };

		if(linesIntersect(line, left) ||
		   linesIntersect(line, top) ||
		   linesIntersect(line, right) ||
		   linesIntersect(line, bottom))
		{
			collision = true;
		}
	}

	return collision;
}

//=============================================================================
// Function: void sendCollisionMessage(int, int, Vector2D)
// Description:
// Sends a collision message to the message system.
// Parameters:
// int entityID - The entity that's being checked.
// int collidingEntityID - The ID of the entity that the first entity collides with.
// Vector2D position - The position of the collision.
//=============================================================================
void CollisionSystem::sendCollisionMessage(int entityID, int collidingEntityID, Vector2D position)
{
	CollisionMessage *message = new CollisionMessage(entityID, collidingEntityID, position);

	MessageSystem::instance()->pushMessage(message);
}

//=============================================================================
// Function: void removeCollisionComponent(int)
// Description:
// Finds and deletes the collision component related to the ID.
// Parameters:
// int entityID - The id of the component to delete.
//=============================================================================
void CollisionSystem::removeCollisionComponent(int entityID)
{
	auto mit = m_components.find(entityID);

	if(mit != m_components.end())
	{
		int gridX = (int)round(mit->second->center().getX());
		int gridY = (int)round(mit->second->center().getY());
		
		m_grid->remove(mit->first, gridX, gridY);

		delete mit->second;
		mit = m_components.erase(mit);
	}
}

void CollisionSystem::cleanup()
{
	auto mit = m_components.begin();

	while (mit != m_components.end())
	{
		delete mit->second;
		mit->second = NULL;

		mit = m_components.erase(mit);

		if (mit != m_components.end())
		{
			mit++;
		}
	}

	delete m_grid;
}