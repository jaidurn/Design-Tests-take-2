#include "CollisionComponent.h"



CollisionComponent::CollisionComponent(pShape shape)
	:Component(COLLISION), m_shape(shape), m_solid(false)
{

}


CollisionComponent::~CollisionComponent()
{
	delete m_shape;
}
