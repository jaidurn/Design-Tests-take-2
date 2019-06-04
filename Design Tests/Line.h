#ifndef _LINE_H
#define _LINE_H
//==========================================================================================
// File Name: Line.h
// Author: Brian Blackmon
// Date Created: 4/5/2019
// Purpose: 
// Introduces lines and line collision.
//==========================================================================================
#include <cmath>
#include "Vector2D.h"

struct Line
{
	Vector2D start;
	Vector2D end;
};

bool isLineUndefined(const Line line); 
float getSlope(const Line line);
Vector2D intersectionPoint(const Line a, const Line b);
bool linesIntersect(const Line a, const Line b);
bool pointOnLine(const Line line, const Vector2D point);
float totalDistance(const Line line);
float totalDistance(const Vector2D start, const Vector2D end);

#endif // !_LINE_H