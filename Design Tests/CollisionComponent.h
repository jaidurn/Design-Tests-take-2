#pragma once
//==========================================================================================
// File Name: CollisionComponent.h
// Author: Brian Blackmon
// Date Created: 3/25/2019
// Purpose: 
// Holds the collision information for an entity.
//==========================================================================================
#include "Component.h"

#include "Rectangle.h"
#include "Circle.h"

typedef Shape::IShape* pShape;
typedef Shape::Circle* pCircle;
typedef Shape::Rectangle* pRectangle;

class CollisionComponent : public Component
{
public:
	CollisionComponent(pShape shape);
	virtual ~CollisionComponent();

	void setRotation(const float rotation) { m_shape->setRotation(rotation); }
	void setCenter(Vector2D center) { m_shape->setCenter(center.getX(), center.getY()); }
	void setSolid(bool isSolid) { m_solid = isSolid; }

	pShape shape() { return m_shape; }
	float rotation() const { return m_shape->rotation(); }
	Vector2D center() { return m_shape->center(); }
	bool isSolid() { return m_solid; }

private:
	pShape m_shape;
	bool m_solid;
};

