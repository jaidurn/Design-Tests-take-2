#include "Rotation.h"

float degreesToRadians(float degrees)
{
	float radians = 0.0;

	const float PI = 3.14159265359f;

	radians = (degrees / 180.0f) * PI;

	return radians;
}

float radiansToDegrees(float radians)
{
	float degrees = 0.0;

	const float PI = 3.14159265359f;

	degrees = (180.0f / PI) * radians;

	if(degrees < 0)
	{
		degrees += 360.0f;
	}

	return degrees;
}

float convertRotationToDegrees(float rotation)
{
	float degrees = 0;

	// Normalize the rotation to 0-100
	degrees = 360 * rotation / 100.0f;

	return degrees;
}

float angleToDegrees(Vector2D origin, Vector2D point)
{
	float xDistance = point.getX() - origin.getX();
	float yDistance = point.getY() - origin.getY();

	return radiansToDegrees(atan2(-xDistance, yDistance));
}

// Vector2D rotatePoint(Vector2D, float)
// Vector2D - The point to rotate
// float - The rotation percentage
// Returns the location of the rotated point.
Vector2D rotatePoint(Vector2D point, Vector2D origin, float rotation)
{
	int rotatedX = 0;
	int rotatedY = 0;
	int tempX = (int)round(point.getX() - origin.getX());
	int tempY = (int)round(point.getY() - origin.getY());

	float degrees = convertRotationToDegrees(rotation);
	float radians = degreesToRadians(degrees);

	float radianCosine = cos(radians);
	float radianSine = sin(radians);

	rotatedX = (int)round((tempX * radianCosine) - (tempY * radianSine));
	rotatedY = (int)round((tempX * radianSine) + (tempY * radianCosine));

	return Vector2D(rotatedX + origin.getX(), rotatedY + origin.getY());
}

Vector2D rotatePoint(int x, int y, int originX, int originY, float rotation)
{
	int rotatedX = 0;
	int rotatedY = 0;
	int tempX = x - originX;
	int tempY = y - originY;

	float degrees = convertRotationToDegrees(rotation);
	float radians = degreesToRadians(degrees);

	float radianCosine = cos(radians);
	float radianSine = sin(radians);

	rotatedX = (int)round((tempX * radianCosine) - (tempY * radianSine));
	rotatedY = (int)round((tempX * radianSine) + (tempY * radianCosine));

	rotatedX += originX;
	rotatedY += originY;

	return Vector2D((float)rotatedX, (float)rotatedY);
}