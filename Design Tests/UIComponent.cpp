#include "UIComponent.h"

UIComponent::UIComponent(UI_TYPE type, int entityID, Vector2D position)
	:Component(UI),
	m_type(type),
	m_entityID(entityID),
	m_position(position),
	m_active(true),
	m_docked(true),
	m_visible(true)
{

}


UIComponent::~UIComponent()
{
}
