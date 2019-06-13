#include "RenderSystem.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "MoveMessage.h"

RenderSystem::~RenderSystem()
{
	cleanUp();
}

void RenderSystem::clear()
{
	if(m_renderer)
	{
		m_renderer->clear();
	}
}

//=============================================================================
// Function: void update()
// Description:
// Clears the renderer, draws current objects, and then renders.
//=============================================================================
void RenderSystem::update()
{
	if (m_renderer)
	{
		// TODO: Add layers into the update system. Go through the render list
		// and render each layer one at a time. For now, just render as they're added.
		drawSprites();
		m_renderer->renderPresent();
	}
}

//=============================================================================
// Function: SpriteComponent* createSprite(ID, string)
// Description:
// Creates a sprite with an ID, from the texture path provided. If there's
// already a sprite with that ID, it just grabs the existing sprite.
// Parameters:
// ID id - The ID of the entity it corresponds with.
// string texturePath - The folder location of the texture to load.
// Output:
// SpriteComponent* - The created sprite component. Returns NULL on failure.
//=============================================================================
SpriteComponent* RenderSystem::createSprite(ID id, std::string texturePath)
{
	SpriteComponent *comp = NULL;
	
	// Make sure the sprite for the ID doesn't exist.
	auto mit = m_sprites.find(id);

	if(mit != m_sprites.end())
	{
		comp = mit->second;
	}
	else
	{
		comp = new SpriteComponent(ResourceManager::instance()->getTexture(texturePath));
		
		// Add the component to the map
		m_sprites.insert(std::make_pair(id, comp));
	}

	return comp;
}

//=============================================================================
// Function: AnimationComponent* createAnimation(ID, float, bool)
// Description:
// Creates an animation component for the ID.
// Parameters:
// ID id - The id to use for the component.
// float speed - The animation speed for the component.
// bool loop - If the animation should loop.
// Output:
// Returns the created animation component.
// Returns NULL on failure.
//=============================================================================
AnimationComponent* RenderSystem::createAnimationComponent(ID id)
{
	AnimationComponent *animation = NULL;

	auto mit = m_animations.find(id);

	if (mit == m_animations.end())
	{
		animation = new AnimationComponent();

		m_animations.insert(std::make_pair(id, animation));
	}
	else
	{
		animation = mit->second;
	}

	return animation;
}

//=============================================================================
// Function: SpriteComponent* getSprite(ID)
// Description:
// Retrieves the sprite related to the ID from the sprite list.
// Parameters:
// ID id - The ID of the sprite to look-up.
// Output:
// SpriteComponent* - A pointer to the sprite component.
// Returns NULL if a sprite isn't found.
//=============================================================================
SpriteComponent* RenderSystem::getSprite(ID id)
{
	SpriteComponent *comp = NULL;

	auto mit = m_sprites.find(id);

	if(mit != m_sprites.end())
	{
		comp = mit->second;
	}

	return comp;
}

//=============================================================================
// Function: AnimationComponent* getAnimation(ID)
// Description:
// Gets the animation component for the ID.
// Parameters:
// ID id - The id to find a component for.
// Output:
// Returns the animation component found.
// Returns NULL if one isn't found.
//=============================================================================
AnimationComponent* RenderSystem::getAnimation(ID id)
{
	auto mit = m_animations.find(id);

	if(mit != m_animations.end())
	{
		return mit->second;
	}

	return NULL;
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Processes the system message.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void RenderSystem::processMessage(IMessage *message)
{
	switch(message->type())
	{
	case IMessage::MessageType::MOVE:
	{
		MoveMessage *move = static_cast<MoveMessage*>(message);
		SpriteComponent *comp = getSprite(move->m_entityID);

		Vector2D position = move->m_newPosition;
		
		comp->setPosition(position);
		break;
	}
	case IMessage::MessageType::ANIMATION_CHANGE:
	{
		AnimationChangeMessage *animation = static_cast<AnimationChangeMessage*>(message);
		processAnimationChange(animation);
	}
	}
}

void RenderSystem::createCamera(SDL_Rect rect, float xScale, float yScale)
{
	if(!m_camera)
	{
		m_camera = new Camera2D(rect, xScale, yScale);
	}
}

void RenderSystem::drawLine(Line line, SDL_Color color)
{
	m_renderer->setColor(color.r, color.g, color.b, color.a);

	Vector2D updatedStart{ line.start.getX() * m_camera->currentScaleX(),
						   line.start.getY() * m_camera->currentScaleY() };
	Vector2D updatedEnd{ line.end.getX() * m_camera->currentScaleX(),
						 line.end.getY() * m_camera->currentScaleY() };

	m_renderer->drawLine(updatedStart, updatedEnd);
	m_renderer->setColor(0, 0, 0, 255);
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up the render objects.
//=============================================================================
void RenderSystem::cleanUp()
{
	// Clean up the sprites
	auto sMit = m_sprites.begin();

	while(sMit != m_sprites.end())
	{
		delete sMit->second;

		sMit = m_sprites.erase(sMit);

		// Make sure our erasing doesn't send us out of the map bounds.
		if(sMit != m_sprites.end())
		{
			sMit++;
		}
	}

	auto aMit = m_animations.begin();

	while(aMit != m_animations.end())
	{
		delete aMit->second;

		aMit = m_animations.erase(aMit);

		if(aMit != m_animations.end())
		{
			aMit++;
		}
	}

	delete m_camera;
}

//=============================================================================
// Function: void drawSprites()
// Description:
// Draws all of the sprites on the screen. 
//=============================================================================
void RenderSystem::drawSprites()
{
	auto mit = m_sprites.begin();

	Vector2D scale(1.0f, 1.0f);

	if(m_camera)
	{
		scale.setX(m_camera->currentScaleX());
		scale.setY(m_camera->currentScaleY());
	}

	while(mit != m_sprites.end())
	{
		if(mit->second)
		{
			SpriteComponent *comp = mit->second;
			Texture *texture = comp->texture();
			Vector2D anchor = comp->anchor();
			Vector2D position = comp->position();

			int xPosition = (int)round((position.getX() * scale.getX()) - ((texture->width() * scale.getX()) / 2));
			int yPosition = (int)round((position.getY() * scale.getY()) - ((texture->height() * scale.getY()) / 2));
			int width = (int)round((float)texture->width() * scale.getX());
			int height = (int)round((float)texture->height() * scale.getY());

			AnimationComponent *animation = getAnimation(mit->first);
			SDL_Rect *clip = NULL;

			if(animation != NULL)
			{
				clip = animation->currentFrame();
				xPosition = (int)round((position.getX() * scale.getX()) - ((clip->w * scale.getX())/ 2));
				yPosition = (int)round((position.getY() * scale.getY()) - ((clip->h * scale.getY())/ 2));
				width = (int)round((float)clip->w * scale.getX());
				height = (int)round((float)clip->h * scale.getY());

				animation->update();
			}

			SDL_Rect destination{xPosition, yPosition, width, height};

			m_renderer->renderTextureEX(
				texture,
				xPosition,
				yPosition,
				&anchor,
				&destination,
				clip,
				comp->rotation());
		}

		mit++;
	}
}

//=============================================================================
// Function: void updateAnimations()
// Description:
// Updates all of the animation components.
//=============================================================================
void RenderSystem::updateAnimations()
{
	auto mit = m_animations.begin();

	while(mit != m_animations.end())
	{
		if(mit->second)
		{
			mit->second->update();
		}

		mit++;
	}
}

void RenderSystem::processAnimationChange(AnimationChangeMessage *message)
{
	AnimationComponent *component = getAnimation(message->m_entityID);

	if(component)
	{
		if(component->currentAnimation() != message->m_name)
		{
			component->setAnimation(message->m_name);

			if(component->currentAnimation() == message->m_name)
			{
				if (message->m_direction != AnimationComponent::DIR_NONE)
				{
					component->setCurrentDirection(message->m_direction);
				}

				if (message->m_frame != -1)
				{
					component->setCurrentFrame(message->m_frame);
				}
			}
		}
		else
		{
			if(message->m_direction != component->currentDirection() && message->m_direction != AnimationComponent::DIR_NONE)
			{
				component->setCurrentDirection(message->m_direction);
			}

			if (message->m_frame != -1)
			{
				component->setCurrentFrame((float)message->m_frame);
			}
		}
	}
}