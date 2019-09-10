#pragma once
//==========================================================================================
// File Name: RenderSystem.h
// Author: Brian Blackmon
// Date Created: 4/20/2019
// Purpose: 
// Handles all of the rendering needs for the application.
// Requires an active renderer to work. 
//==========================================================================================
#include <map>
#include <string>
#include "Line.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "IMessage.h"
#include "MessageSystem.h"
#include "Camera2D.h"
#include "TextureEffect.h"
#include "TextComponent.h"
#include "Grid.h"

class Renderer;
class AnimationChangeMessage;
class MoveMessage;

typedef int ID;

class RenderSystem
{
public:
	enum RenderLayers
	{
		RENDER_BACKGROUND,
		RENDER_FOREGROUND0,
		RENDER_FOREGROUND1,
		RENDER_FOREGROUND2,
		RENDER_UI_BACKGROUND,
		RENDER_UI_MIDGROUND,
		RENDER_UI_FOREGROUND
	};

	static RenderSystem *instance()
	{
		static RenderSystem *instance = new RenderSystem();

		return instance;
	}

	~RenderSystem();

	void clear();
	void update(float delta);

	// TODO: Maybe take component creation away from the system, but have the system
	// have control of registering and destroying them.
	SpriteComponent* createSprite(ID id, std::string texturePath, Vector2D position);
	AnimationComponent* createAnimationComponent(ID id);
	TextComponent* createTextComponent(ID id, string text, Font *font, SDL_Color color, Uint32 wrapWidth, Vector2D position);

	SpriteComponent* getSprite(ID id);
	AnimationComponent* getAnimation(ID id);
	TextComponent *getText(ID id);
	TextureEffect* getEffect(ID id);

	void setSpriteLayer(ID spriteID, RenderLayers layer);

	void processMessage(IMessage *message);

	void setRenderer(Renderer *renderer) { m_renderer = renderer; }

	void createCamera(SDL_Rect rect, float xScale, float yScale);

	void drawLine(Line line, SDL_Color color);

	void createTextureEffect(int entityID, TextureEffect::EffectType type, SDL_Color endColor, SDL_BlendMode blendMode, float duration, float speed);
//	void deleteTextureEffect(int entityID);

	void setCameraTarget(int targetID);

	Camera2D* camera() { return m_camera; }

private:
	const int m_LAYER_COUNT = 7;
	const int m_GRID_SIZE = 256;

	RenderSystem()
		:m_renderer(NULL), m_camera(NULL)
	{
		for(int i = 0; i < m_LAYER_COUNT; i++)
		{
			m_layers.push_back(new Grid(0, 0, 1280, 720, m_GRID_SIZE));
		}
	}

	Renderer *m_renderer;

	Camera2D *m_camera;

	std::map<ID, SpriteComponent*> m_sprites;
	std::map<ID, AnimationComponent*> m_animations;
	std::map<ID, TextComponent*> m_texts;
	std::map<ID, TextureEffect*> m_effects;

	std::vector<Grid*> m_layers;

	void cleanUp();
	void drawSprites(float delta, int layer);
	void drawUI(float delta);
	void drawText(float delta);
	void updateAnimations();
	// TODO: Add functionality to render our collision objects.

	void processAnimationChange(AnimationChangeMessage *message);
	void processMoveMessage(MoveMessage *message);

	void removeSprite(int entityID);
	void removeAnimation(int entityID);
	void removeText(int entityID);
};

