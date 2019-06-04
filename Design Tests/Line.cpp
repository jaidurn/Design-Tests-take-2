#include "Line.h"
#include <iostream>

// Checks if a line has an undefined slope.
bool isLineUndefined(const Line line)
{
	return (line.start.getX() == line.end.getX());
}

// Calculates the slope of a line.
float getSlope(const Line line)
{
	float slope = 0;

	if (!isLineUndefined(line))
	{
		Vector2D distance = line.end - line.start;

		slope = distance.getY() / distance.getX();
	}

	return slope;
}

// Gets the point at which two lines intersect
Vector2D intersectionPoint(const Line a, const Line b)
{
	Vector2D point{ 0, 0 };

	// The if both lines aren't undefined
	if (!isLineUndefined(a) && !isLineUndefined(b))
	{
		float slopeA = getSlope(a);
		float slopeB = getSlope(b);
		float yInterceptA = a.start.getY() - (a.start.getX() * slopeA);
		float yInterceptB = b.start.getY() - (b.start.getX() * slopeB);

		float tempPointX = ((slopeA * a.start.getX()) - (slopeB * b.start.getX()) + b.start.getY() - a.start.getY()) / (slopeA - slopeB);
		float tempPointY = (slopeA * (tempPointX - a.start.getX())) + a.start.getY();

		tempPointX = round(tempPointX);
		tempPointY = round(tempPointY);

		point.setX(tempPointX);
		point.setY(tempPointY);
	}
	else
	{
		if (isLineUndefined(a) && !isLineUndefined(b))
		{
			float slopeB = getSlope(b);
			float intersectB = b.start.getY() - (slopeB * b.start.getX());

			float tempPointX = a.start.getX();
			float tempPointY = (slopeB * a.start.getX()) + intersectB;

			point.setX(tempPointX);
			point.setY(tempPointY);
		}
		else if (!isLineUndefined(a) && isLineUndefined(b))
		{
			float slopeA = getSlope(a);

			float intersectA = a.start.getY() - (slopeA * a.start.getX());

			float tempPointX = b.start.getX();
			float tempPointY = (slopeA * b.start.getX()) + intersectA;

			point.setX(tempPointX);
			point.setY(tempPointY);
		}
	}

	return point;
}

// Checks to see if two line segments intersect
bool linesIntersect(const Line a, const Line b)
{
	bool intersection = false;

	if (!isLineUndefined(a) && !isLineUndefined(b))
	{
		float slopeA = getSlope(a);
		float slopeB = getSlope(b);

		// Find the Y intersepts of each line
		float interseptA = a.start.getY() - (slopeA * a.start.getX());
		float interseptB = b.start.getY() - (slopeB * b.start.getX());

		// If we have the same slope and the same intercept
		if ((slopeA == slopeB) && (interseptA == interseptB))
		{
			// Check if the lines are actually touching
			if (pointOnLine(a, b.start) || pointOnLine(a, b.end))
			{
				intersection = true;
			}
		}
		// Make sure we're not checking a parallel line for intersecting points
		else if (slopeA != slopeB)
		{
			// Get the intersection point of the line
			Vector2D intersect = intersectionPoint(a, b);

			// Make sure the point is on the line segment
			if (pointOnLine(a, intersect) && pointOnLine(b, intersect))
			{
				intersection = true;
			}
		}

	}
	else if((!isLineUndefined(a) && isLineUndefined(b)) ||
			(isLineUndefined(a) && !isLineUndefined(b)))
	{
		if (isLineUndefined(a))
		{
			// Get the intersection point of the line
			Vector2D intersect = intersectionPoint(a, b);

			if (pointOnLine(a, intersect) && pointOnLine(b, intersect))
			{
				intersection = true;
			}

		}
		else if (isLineUndefined(b))
		{
			// Get the intersection point of the line
			Vector2D intersect = intersectionPoint(a, b);

			if (pointOnLine(a, intersect) && pointOnLine(b, intersect))
			{
				intersection = true;
			}
		}
	}
	else
	{
		// If the lines are both undefined and share the same x coordinates
		if (a.start.getX() == b.start.getX())
		{
			// Check if the lines are actually touching
			if (pointOnLine(a, b.start) || pointOnLine(a, b.end))
			{
				intersection = true;
			}
		}
	}

	return intersection;
}

// Checks if a point is on a line
//TODO: Fix this function!
bool pointOnLine(const Line line, const Vector2D point)
{
	bool onLine = false;

	if (!isLineUndefined(line))
	{
		if(getSlope(line) == 0)
		{
			// Find the point on a line with no slope
			float smallestPointX = line.start.getX();
			float largestPointX = line.end.getX();

			if(line.end.getX() < line.start.getX())
			{
				smallestPointX = line.end.getX();
				largestPointX = line.start.getX();
			}

			if(smallestPointX <= point.getX() && point.getX() <= largestPointX)
			{
				onLine = true;
			}
		}
		else
		{
			float distanceA = totalDistance(line.start, point);
			float distanceB = totalDistance(line.end, point);

			float distanceTotal = distanceA + distanceB;

			if (distanceTotal == totalDistance(line))
			{
				onLine = true;
			}
		}
	}
 	else
	{
		if(line.start.getX() == point.getX())
		{
			//Find the smaller part of the line
			if(line.start.getY() < line.end.getY())
			{
				if(line.start.getY() <= point.getY() && point.getY() <= line.end.getY())
				{
					onLine = true;
				}
			}
			else
			{
				if(line.end.getY() <= point.getY() && point.getY() <= line.start.getY())
				{
					onLine = true;
				}
			}
		}
	}


	return onLine;
}

// Returns the total distance of the line
float totalDistance(const Line line)
{
	float totalDistance = 0;

	float distanceX = line.end.getX() - line.start.getX();
	float distanceY = line.end.getY() - line.start.getY();

	if (distanceX < 0) { distanceX *= -1; }
	if (distanceY < 0) { distanceY *= -1; }

	totalDistance = distanceX + distanceY;

	return totalDistance;
}

// Returns the total distance between the two points
float totalDistance(const Vector2D start, const Vector2D end)
{
	Line line{ start, end };

	return totalDistance(line);
}