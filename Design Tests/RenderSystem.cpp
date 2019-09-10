#include "RenderSystem.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "AnimationChangeMessage.h"
#include "MoveMessage.h"
#include "EntityDestroyMessage.h"
#include "MoveMessage.h"
#include "CameraMoveMessage.h"
#include <iostream>

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
void RenderSystem::update(float delta)
{
	if (m_renderer)
	{
		for (int i = 0; i < (int)RENDER_UI_BACKGROUND; i++)
		{
			drawSprites(delta, i);
		}

		drawUI(delta);
		drawText(delta);

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
// Vector2D position - The position to create the sprite at.
// Output:
// SpriteComponent* - The created sprite component. Returns NULL on failure.
//=============================================================================
SpriteComponent* RenderSystem::createSprite(ID id, 
	std::string texturePath, 
	Vector2D position)
{
	SpriteComponent *comp = NULL;
	
	// Make sure the sprite for the ID doesn't exist.
	auto mit = m_sprites.find(id);

	if(mit != m_sprites.end())
	{
		std::cout << "Sprite already exists for: " << id << "!\n";
		comp = mit->second;
	}
	else
	{
		comp = new SpriteComponent(ResourceManager::instance()->getTexture(texturePath));
		
		comp->setPosition(position);

		m_layers[comp->layer()]->add(id, (int)round(position.getX()), (int)round(position.getY()));

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

	if (mit != m_animations.end())
	{
		animation = mit->second;
	}
	else
	{
		animation = new AnimationComponent();

		m_animations.insert(std::make_pair(id, animation));
	}

	return animation;
}

//=============================================================================
// Function: TextComponent *createTextComponent(ID, string, 
//												Font*, SDL_Color,
//												Uint32, Vector2D)
// Description:
// Creates a text component, allowing for the rendering of text.
// Parameters:
//
// ID id - The entity id to create the text component for.
// string text - The text to initialize it with.
//
// Font *font - The font to use.
//
// SDL_Color color - The color of the text.
//
// Uint32 wrapWidth - The width in pixels to wrap the text after.
// Use 0 if you don't want to wrap anything.
//
// Vector2D position - The position to create the text component at.
//
// Output:
// Returns a pointer to the created text component on success.
// Returns NULL on failure.
//=============================================================================
TextComponent* RenderSystem::createTextComponent(ID id, 
	string text, 
	Font *font, 
	SDL_Color color,
	Uint32 wrapWidth,
	Vector2D position)
{
	TextComponent *textComp = NULL;

	auto mit = m_texts.find(id);

	if(mit != m_texts.end())
	{
		textComp = mit->second;

		// Update the existing text component
		textComp->setText(text);
		textComp->setFont(font);
		textComp->setColor(color);
		textComp->setWrapWidth(wrapWidth);
		textComp->setPosition(position);
	}
	else
	{
		textComp = new TextComponent(font, color, text, wrapWidth, position);

		if(textComp)
		{
			m_texts.insert(std::make_pair(id, textComp));
		}
	}

	return textComp;
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
// Function: TextComponent* getText(ID)
// Description:
// Finds and returns the text component related to the specified ID.
// Parameters:
// ID id - The entity id of the text component to find.
// Output:
// Returns a pointer to the text component if found.
// Returns NULL if it doesn't exist.
//=============================================================================
TextComponent* RenderSystem::getText(ID id)
{
	TextComponent *text = NULL;

	auto mit = m_texts.find(id);

	if(mit != m_texts.end())
	{
		text = mit->second;
	}

	return text;
}

//=============================================================================
// Function: TextureEffect* getEffect(ID)
// Description:
// Gets the texture effect for the ID.
// Parameters:
// ID id - The id of the effect to get.
// Output:
// Returns the found texture effect on success.
// Returns NULL on failure.
//=============================================================================
TextureEffect* RenderSystem::getEffect(ID id)
{
	TextureEffect *effect = NULL;

	auto mit = m_effects.find(id);

	if(mit != m_effects.end())
	{
		effect = mit->second;
	}

	return effect;
}

//=============================================================================
// Function: void setSpriteLayer(ID, RenderLayers)
// Description:
// Removes the sprite from the previous layer it was on
// and changes it to the new layer.
// Parameters:
// ID spriteID - The id of the sprite to change layers.
// RenderLayers layer - The layer to change the sprite to.
//=============================================================================
void RenderSystem::setSpriteLayer(ID spriteID, RenderLayers layer)
{
	// Find the sprite's current layer and remove it.
	SpriteComponent *sprite = getSprite(spriteID);

	if(sprite)
	{
		int oldLayer = sprite->layer();

		if (oldLayer < 0) { oldLayer = 0; }
		else if (m_LAYER_COUNT <= oldLayer) { oldLayer = m_LAYER_COUNT - 1; }

		m_layers[oldLayer]->remove(spriteID, (int)round(sprite->position().getX()), (int)round(sprite->position().getY()));

		int workingLayer = layer;

		if (workingLayer < 0) 
		{ 
			std::cout << "Error: Sprite working layer less than 0!\n";
			workingLayer = 0; 
		}
		else if (m_LAYER_COUNT <= workingLayer) 
		{
			std::cout << "Error: Sprite working layer greater than the layer count!\n";
			workingLayer = m_LAYER_COUNT - 1; 
		}

		m_layers[workingLayer]->add(spriteID, (int)round(sprite->position().getX()), (int)round(sprite->position().getY()));

		sprite->setLayer(workingLayer);
	}
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
	case IMessage::MOVE:
	{
		MoveMessage *move = static_cast<MoveMessage*>(message);

		processMoveMessage(move);

		break;
	}
	case IMessage::ANIMATION_CHANGE:
	{
		AnimationChangeMessage *animation = static_cast<AnimationChangeMessage*>(message);

		processAnimationChange(animation);

		break;
	}
	case IMessage::ENTITY_DESTROY:
	{
		EntityDestroyMessage *destroy = static_cast<EntityDestroyMessage*>(message);

		removeSprite(destroy->m_entityID);
		removeAnimation(destroy->m_entityID);
		removeText(destroy->m_entityID);

		break;
	}
	}
}

//=============================================================================
// Function: void createCamera(SDL_Rect, float, float)
// Description:
// Creates a camera with the specified scale if a camera doesn't
// already exist.
// Parameters:
// SDL_Rect rect - The rectangle containing the size of the camera.
// float xScale - The x scale of the camera.
// float yScale - The y scale of the camera.
//=============================================================================
void RenderSystem::createCamera(SDL_Rect rect, 
	float xScale, 
	float yScale)
{
	if(!m_camera)
	{
		m_camera = new Camera2D(rect, xScale, yScale);
	}
}

//=============================================================================
// Function: void drawLine(Line, SDL_Color)
// Description:
// Draws a line scaled with the camera in the specified color.
// Parameters:
// Line line - The line to draw.
// SDL_Color color - The color of the line.
//=============================================================================
void RenderSystem::drawLine(Line line, SDL_Color color)
{
	int xOffset = 0;
	int yOffset = 0;

	if(m_camera)
	{
		xOffset = m_camera->getX();
		yOffset = m_camera->getY();
	}

	m_renderer->setColor(color.r, color.g, color.b, color.a);

	Vector2D updatedStart{ line.start.getX() * m_camera->currentScaleX() - xOffset,
						   line.start.getY() * m_camera->currentScaleY() - yOffset};
	Vector2D updatedEnd{ line.end.getX() * m_camera->currentScaleX() - xOffset,
						 line.end.getY() * m_camera->currentScaleY() - yOffset};

	m_renderer->drawLine(updatedStart, updatedEnd);
	m_renderer->setColor(0, 0, 0, 255);
}

//=============================================================================
// Function: void createTextureEffect(int, EffectType, SDL_Color, float, float)
// Description:
// Creates a texture effect with the specified parameters and adds it to the entity.
// Parameters:
// int entityID - The ID of the entity to apply the effect to.
// EffectType type - The type of the effect to create.
// SDL_Color color - The end color of the effect.
// float duration - The duration length of the effect.
// float speed - The speed of the effect.
//=============================================================================
void RenderSystem::createTextureEffect(int entityID, 
	TextureEffect::EffectType type, 
	SDL_Color endColor, 
	SDL_BlendMode blendMode, 
	float duration, 
	float speed)
{
	SpriteComponent *component = getSprite(entityID);

	if (component)
	{
		auto mit = m_effects.find(entityID);

		if(mit == m_effects.end())
		{
			TextureEffect *effect = new TextureEffect(component, type, endColor, blendMode, duration, speed);

			m_effects.insert(std::make_pair(entityID, effect));
		}
		else
		{
			TextureEffect *effect = mit->second;

			effect->setEndColor(endColor);
			effect->setDuration(duration);
			effect->setSpeed(speed);
			effect->setBlendMode(blendMode);

			effect->reset();
		}
	}
}

//=============================================================================
// Function: void setCameraTarget(int)
// Description:
// Sets the cameras focus target to the specified ID and moves the camera
// to center it on the entity.
// Parameters:
// int targetID - The ID of the target entity.
//=============================================================================
void RenderSystem::setCameraTarget(int targetID)
{
	SpriteComponent *sprite = getSprite(targetID);

	if(sprite)
	{
		m_camera->setCurrentTarget(targetID);

		Vector2D oldPosition{ (float)m_camera->getX() + (m_camera->getWidth() / 2), (float)m_camera->getY() + (m_camera->getHeight() / 2) };

		m_camera->setX((int)(sprite->position().getX() * m_camera->currentScaleX() - ((m_camera->getWidth() / 2))));
		m_camera->setY((int)(sprite->position().getY() * m_camera->currentScaleY() - ((m_camera->getHeight() / 2))));

		Vector2D newPosition{ (float)(m_camera->getX() + (m_camera->getWidth() / 2)),
					  (float)(m_camera->getY() + (m_camera->getHeight() / 2)) };

		CameraMoveMessage *move = new CameraMoveMessage(oldPosition, newPosition);

		MessageSystem::instance()->pushMessage(move);
	}
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

	auto eMit = m_effects.begin();

	while(eMit != m_effects.end())
	{
		delete eMit->second;

		eMit = m_effects.erase(eMit);

		if(eMit != m_effects.end())
		{
			eMit++;
		}
	}

	auto tMit = m_texts.begin();

	while(tMit != m_texts.end())
	{
		delete tMit->second;
		tMit->second = NULL;

		tMit = m_texts.erase(tMit);

		if(tMit != m_texts.end())
		{
			tMit++;
		}
	}

	for(int i = 0; i < m_LAYER_COUNT; i++)
	{
		delete m_layers[i];
	}

	delete m_camera;
}

//=============================================================================
// Function: void drawSprites(float)
// Description:
// Draws all of the sprites on the screen.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void RenderSystem::drawSprites(float delta, int layer)
{
	auto mit = m_sprites.begin();

	Vector2D scale(1.0f, 1.0f);

	int offsetX = 0;
	int offsetY = 0;

	if (m_camera)
	{
		scale.setX(m_camera->currentScaleX());
		scale.setY(m_camera->currentScaleY());
		offsetX = m_camera->getX();
		offsetY = m_camera->getY();
	}

	int startingX = (int)round(m_camera->getX() / (m_GRID_SIZE * scale.getX()));
	int startingY = (int)round(m_camera->getY() / (m_GRID_SIZE * scale.getY()));
	int endingX = (int)ceil((float)(m_camera->getX() + m_camera->getWidth()) / ((float)m_GRID_SIZE * scale.getX()));
	int endingY = (int)ceil((float)(m_camera->getY() + m_camera->getHeight()) / ((float)m_GRID_SIZE * scale.getY()));

	if (0 < startingX) { startingX -= 1; }
	if (0 < startingY) { startingY -= 1; }

	int xDiff = endingX - startingX;
	int yDiff = endingY - startingY;

	for (int i = 0; i < xDiff; i++)
	{
		for (int j = 0; j < yDiff; j++)
		{
			Node<int> *cell = m_layers[layer]->getCell(startingX + i, startingY + j);

			while (cell)
			{
				SpriteComponent *sprite = getSprite(cell->data());

				if (sprite && sprite->visible())
				{
					TextureEffect *effect = getEffect(cell->data());

					Texture *texture = sprite->texture();

					Vector2D anchor = sprite->anchor();
					Vector2D position = sprite->position();

					SDL_Rect *clip = &sprite->clip();
					SDL_Rect *renderSize = &sprite->renderSize();

					int width = renderSize->w;
					int height = renderSize->h;

					AnimationComponent *animation = getAnimation(cell->data());

					if (animation != NULL)
					{
						// If the render size is just the clip size,
						// change the render size to be the animation's clip size.
						if(renderSize->w == clip->w && renderSize->h == clip->h)
						{
							width = animation->currentFrame()->w;
							height = animation->currentFrame()->h;
						}

						clip = animation->currentFrame();
						animation->update();
					}

					int xPosition = (int)round((position.getX()) - ((width) / 2)) - offsetX;
					int yPosition = (int)round((position.getY()) - ((height) / 2)) - offsetY;

//					float spriteTopLeftX = (float)(position.getX()) - (float)((width / 2));
//					float spriteTopLeftY = (position.getY()) - (float)((height / 2));
//					float spriteBottomRightX = (position.getX()) + (float)((width / 2));
//					float spriteBottomRightY = (position.getY()) + (float)((height / 2));

					if (sprite->scaleable())
					{
						xPosition = (int)round((position.getX() * scale.getX()) - ((width * scale.getX()) / 2)) - offsetX;
						yPosition = (int)round((position.getY() * scale.getY()) - ((height * scale.getY()) / 2)) - offsetY;
						width = (int)round((float)width * scale.getX());
						height = (int)round((float)height * scale.getY());

//						spriteTopLeftX = (float)(position.getX() * scale.getX()) - (float)((width / 2) * scale.getX());
//						spriteTopLeftY = (position.getY() * scale.getY()) - (float)((height / 2) * scale.getY());
//						spriteBottomRightX = (position.getX() * scale.getX()) + (float)((width / 2) * scale.getX());
//						spriteBottomRightY = (position.getY() * scale.getY()) + (float)((height / 2) * scale.getY());
					}

					bool visible = true;

					/*
					TODO: Check if this is too much optimization or not.
					This could be a potential slowdown from all of the
					calculations required to figure it out.
					if (spriteBottomRightX < m_camera->getX())
					{
						visible = false;
					}
					else if (m_camera->getX() + m_camera->getWidth() < spriteTopLeftX)
					{
						visible = false;
					}
					else if (spriteBottomRightY < m_camera->getY())
					{
						visible = false;
					}
					else if (m_camera->getY() + m_camera->getHeight() < spriteTopLeftY)
					{
						visible = false;
					}*/

					if (visible)
					{
						SDL_Color colorMod = sprite->colorMod();
						SDL_BlendMode blendMode = sprite->blendMode();

						if (effect)
						{
							if (!effect->started())
							{
								effect->start();
							}

							effect->applyEffect(delta);
							colorMod = effect->currentColor();
							blendMode = effect->blendMode();
						}

						SDL_SetTextureColorMod(texture->texture(), colorMod.r, colorMod.g, colorMod.b);
						SDL_SetTextureAlphaMod(texture->texture(), colorMod.a);
						SDL_SetTextureBlendMode(texture->texture(), blendMode);

						SDL_Rect destination{ xPosition, yPosition, width, height };

						m_renderer->renderTextureEX(
							texture,
							xPosition,
							yPosition,
							&anchor,
							&destination,
							clip,
							sprite->rotation());
					}
				}

				cell = cell->m_next;
			}
		}
	}
}

//=============================================================================
// Function: void drawUI(float)
// Description:
// Draws all of the UI sprites on the screen. UI sprites are different 
// from normal sprites because it doesn't get scaled with the camera.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void RenderSystem::drawUI(float delta)
{
	auto mit = m_sprites.begin();

	int offsetX = 0;
	int offsetY = 0;

	if (m_camera)
	{
		offsetX = m_camera->getX();
		offsetY = m_camera->getY();
	}

	int startingX = (int)round(m_camera->getX() / m_GRID_SIZE);
	int startingY = (int)round(m_camera->getY() / m_GRID_SIZE);
	int endingX = (int)ceil((float)(m_camera->getX() + m_camera->getWidth()) / (float)m_GRID_SIZE);
	int endingY = (int)ceil((float)(m_camera->getY() + m_camera->getHeight()) / (float)m_GRID_SIZE);

	if (0 < startingX) { startingX -= 1; }
	if (0 < startingY) { startingY -= 1; }

	int xDiff = endingX - startingX;
	int yDiff = endingY - startingY;

	for (int layer = (int)RENDER_UI_BACKGROUND; layer < m_LAYER_COUNT; layer++)
	{
		for (int i = 0; i < xDiff; i++)
		{
			for (int j = 0; j < yDiff; j++)
			{
				Node<int> *cell = m_layers[layer]->getCell(startingX + i, startingY + j);

				while (cell)
				{
					SpriteComponent *sprite = getSprite(cell->data());

					if (sprite && sprite->visible())
					{
						TextureEffect *effect = getEffect(cell->data());

						Texture *texture = sprite->texture();
						Vector2D anchor = sprite->anchor();
						Vector2D position = sprite->position();

						AnimationComponent *animation = getAnimation(cell->data());
						SDL_Rect *clip = &sprite->clip();
						SDL_Rect *renderSize = &sprite->renderSize();

						int width = renderSize->w;
						int height = renderSize->h;

						if (animation != NULL)
						{
							if (renderSize->w == clip->w && renderSize->h == clip->h)
							{
								width = animation->currentFrame()->w;
								height = animation->currentFrame()->h;
							}

							clip = animation->currentFrame();
							animation->update();
						}

						int xPosition = (int)round((position.getX()) - ((renderSize->w) / 2)) - offsetX;
						int yPosition = (int)round((position.getY()) - ((renderSize->h) / 2)) - offsetY;

						SDL_Color colorMod = sprite->colorMod();
						SDL_BlendMode blendMode = sprite->blendMode();

						if (effect)
						{
							if (!effect->started())
							{
								effect->start();
							}

							effect->applyEffect(delta);
							colorMod = effect->currentColor();
							blendMode = effect->blendMode();
						}

						SDL_SetTextureColorMod(texture->texture(), colorMod.r, colorMod.g, colorMod.b);
						SDL_SetTextureAlphaMod(texture->texture(), colorMod.a);
						SDL_SetTextureBlendMode(texture->texture(), blendMode);

						SDL_Rect destination{ xPosition, yPosition, width, height };

						m_renderer->renderTextureEX(
							texture,
							xPosition,
							yPosition,
							&anchor,
							&destination,
							clip,
							sprite->rotation());
					}

					cell = cell->m_next;
				}
			}
		}
	}
}

//=============================================================================
// Function: void drawText(float)
// Description:
// Renders all of the existing text components.
// Parameters:
// float delta - The amount of time that's passed since the last update.
//=============================================================================
void RenderSystem::drawText(float delta)
{
	auto mit = m_texts.begin();

	while(mit != m_texts.end())
	{
		TextComponent *text = mit->second;

		if (text->getVisible())
		{
			if (text->getTexture())
			{
				Vector2D targetPosition(text->getPosition().getX() - (text->getTexture()->width() / 2),
					text->getPosition().getY() - (text->getTexture()->height() / 2));

				if (m_camera)
				{
					targetPosition.setX(targetPosition.getX() - (float)m_camera->getX());
					targetPosition.setY(targetPosition.getY() - (float)m_camera->getY());
				}

				m_renderer->renderTexture(text->getTexture(),
					(int)targetPosition.getX(),
					(int)targetPosition.getY(),
					NULL);
			}
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

//=============================================================================
// Function: void processAnimationChange(AnimationChangeMessage*)
// Description:
// Processes animation change messages and updates the animation
// component related to it.
// Parameters:
// AnimationChangeMessage *message - The animation change message to
// process.
//=============================================================================
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
					component->setCurrentFrame((float)(message->m_frame));
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

//=============================================================================
// Function: void processMoveMessage(MoveMessage*)
// Description:
// Processes the move message and updates the positions of the
// related components.
// Parameters:
// MoveMessage *message - The move message to process.
//=============================================================================
void RenderSystem::processMoveMessage(MoveMessage *message)
{
	SpriteComponent *comp = getSprite(message->m_entityID);
	TextComponent *text = getText(message->m_entityID);

	Vector2D position = message->m_newPosition;

	if (comp)
	{
		m_layers[comp->layer()]->move(message->m_entityID, (int)round(comp->position().getX()), (int)round(comp->position().getY()), (int)round(position.getX()), (int)round(position.getY()));

		if (m_camera)
		{
			if (message->m_entityID == m_camera->currentTarget())
			{
				if (m_camera->boundingBox())
				{
					float cameraLeftX = m_camera->boundingBox()->center().getX() - (m_camera->boundingBox()->width() / 2);
					float cameraRightX = m_camera->boundingBox()->center().getX() + (m_camera->boundingBox()->width() / 2);
					float cameraTopY = m_camera->boundingBox()->center().getY() - (m_camera->boundingBox()->height() / 2);
					float cameraBottomY = m_camera->boundingBox()->center().getY() + (m_camera->boundingBox()->height() / 2);

					float cameraMoveX = 0;
					float cameraMoveY = 0;

					float spriteX = comp->position().getX() * m_camera->currentScaleX();
					float spriteY = comp->position().getY() * m_camera->currentScaleY();

					if (spriteX < cameraLeftX)
					{
						cameraMoveX = spriteX - cameraLeftX;
					}
					else if (cameraRightX < spriteX)
					{
						cameraMoveX = spriteX - cameraRightX;
					}

					if (spriteY < cameraTopY)
					{
						cameraMoveY = spriteY - cameraTopY;
					}
					else if (cameraBottomY < spriteY)
					{
						cameraMoveY = spriteY - cameraBottomY;
					}

					if (cameraMoveX != 0 || cameraMoveY != 0)
					{
						Vector2D oldPosition{ (float)m_camera->getX(), (float)m_camera->getY() };
						Vector2D newPosition{ (float)(m_camera->getX() + (int)cameraMoveX),
											  (float)(m_camera->getY() + (int)cameraMoveY) };

						m_camera->setX(m_camera->getX() + (int)cameraMoveX);
						m_camera->setY(m_camera->getY() + (int)cameraMoveY);

						CameraMoveMessage *cameraMove = new CameraMoveMessage(oldPosition, newPosition);

						MessageSystem::instance()->pushMessage(cameraMove);
					}
				}
				else
				{
					int cameraMoveX = (int)(comp->position().getX() * m_camera->currentScaleX() - ((m_camera->getWidth() / 2)));
					int cameraMoveY = (int)(comp->position().getY() * m_camera->currentScaleY() - ((m_camera->getHeight() / 2)));

					if (cameraMoveX != m_camera->getX() || cameraMoveY != m_camera->getY())
					{
						Vector2D oldPosition{ (float)m_camera->getX(), (float)m_camera->getY() };
						Vector2D newPosition{ (float)(cameraMoveX),
											  (float)(cameraMoveY) };

						m_camera->setX(cameraMoveX);
						m_camera->setY(cameraMoveY);

						CameraMoveMessage *cameraMove = new CameraMoveMessage(oldPosition, newPosition);

						MessageSystem::instance()->pushMessage(cameraMove);
					}
				}

			}
		}

		comp->setPosition(position);
	}

	if (text)
	{
		text->setPosition(position);
	}
}

//=============================================================================
// Function: void removeSprite(int)
// Description:
// Finds and removes the sprite related to the entity ID.
// Parameters:
// int entityID - The ID of the sprite to remove.
//=============================================================================
void RenderSystem::removeSprite(int entityID)
{
	auto mit = m_sprites.find(entityID);
		if(mit != m_sprites.end())
	{
		m_layers[mit->second->layer()]->remove(mit->first, (int)mit->second->position().getX(), (int)mit->second->position().getY());

		delete mit->second;

		mit = m_sprites.erase(mit);
	}
}

//=============================================================================
// Function: void removeAnimation(int)
// Description:
// Finds and removes the animation related to the entity ID.
// Parameters:
// int entityID - The ID of the animation to remove.
//=============================================================================
void RenderSystem::removeAnimation(int entityID)
{
	auto mit = m_animations.find(entityID);

	if(mit != m_animations.end())
	{
		delete mit->second;

		mit = m_animations.erase(mit);
	}
}

//=============================================================================
// Function: void removeText(int entityID)
// Description:
// Finds and deletes the text component related to the entityID.
// Parameters:
// int entityID - The id of the component to find.
//=============================================================================
void RenderSystem::removeText(int entityID)
{
	auto mit = m_texts.find(entityID);

	if(mit != m_texts.end())
	{
		delete mit->second;
		mit->second = NULL;

		mit = m_texts.erase(mit);
	}
}