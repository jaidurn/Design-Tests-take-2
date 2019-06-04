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
#include "AnimationChangeMessage.h"
#include "Camera2D.h"

class Renderer;

typedef int ID;

class RenderSystem
{
public:
	static RenderSystem *instance()
	{
		static RenderSystem *instance = new RenderSystem();

		return instance;
	}

	~RenderSystem();

	void clear();
	void update();

	// TODO: Maybe take component creation away from the system, but have the system
	// have control of registering and destroying them.
	SpriteComponent* createSprite(ID id, std::string texturePath);
	AnimationComponent* createAnimationComponent(ID id);

	SpriteComponent* getSprite(ID id);
	AnimationComponent* getAnimation(ID id);

	void processMessage(IMessage *message);

	void setRenderer(Renderer *renderer) { m_renderer = renderer; }

	void createCamera(SDL_Rect rect, float xScale, float yScale);

	void drawLine(Line line, SDL_Color color);

	Camera2D* camera() { return m_camera; }

private:
	RenderSystem()
		:m_renderer(NULL), m_camera(NULL)
	{
	}

	Renderer *m_renderer;

	Camera2D *m_camera;
	// TODO: Create individual cache systems for all of the components.
	std::map<ID, SpriteComponent*> m_sprites;
	std::map<ID, AnimationComponent*> m_animations;

	void cleanUp();
	void drawSprites();
	void updateAnimations();
	// TODO: Add functionality to render our collision objects.

	void processAnimationChange(AnimationChangeMessage *message);
};

