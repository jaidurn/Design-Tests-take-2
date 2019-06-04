#include "SpriteComponent.h"
#include <cassert>

SpriteComponent::SpriteComponent(Texture *texture)
	:Component(SPRITE), m_texture(texture), m_anchor(-1,-1), m_position(0, 0), m_rotation(0.0f)
{
	// Default the anchor to the center
	if (texture) { m_anchor = m_texture->center(); }
}

SpriteComponent::~SpriteComponent()
{
	// TODO: Tell texture something is unsubscribing!
	m_texture = NULL;
}

//=============================================================================
// Function: void setTexture(Texture)
// Description:
// Sets the texture pointer to the texture provided. If the anchor hasn't been
// set already, it sets the anchor to the center of the texture.
// Parameters:
// Texture *texture - The texture to set the member texture to.
//=============================================================================
void SpriteComponent::setTexture(Texture *texture)
{
	if (texture)
	{
		m_texture = texture;

		// Make sure the anchor gets initialized.
		if(m_anchor.getX() < 0 || m_anchor.getY() < 0)
		{
			m_anchor = m_texture->center();
		}
	}
}