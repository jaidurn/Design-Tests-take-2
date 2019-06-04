#ifndef _ROTATION_H
#define _ROTATION_H
//==========================================================================================
// File Name: Rotation.h
// Author: Brian Blackmon
// Date Created: 4/5/2019
// Purpose: 
// Holds information on radian to degree conversion and other rotation
// functions.
//==========================================================================================
#include <cmath>
#include <iostream>
#include "Vector2D.h"

float degreesToRadians(float degrees); 
float radiansToDegrees(float radians);
float convertRotationToDegrees(float rotation);
Vector2D rotatePoint(Vector2D point, Vector2D origin, float rotation);
Vector2D rotatePoint(int x, int y, int originX, int originY, float rotation);

#endif //_ROTATION_H