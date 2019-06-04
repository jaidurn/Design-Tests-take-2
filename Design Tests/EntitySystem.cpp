#include "EntitySystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"

EntitySystem::~EntitySystem()
{

}

//=============================================================================
// Function: void initialize(string)
// Description:
// Initializes the entity system if it hasn't already. Initialization
// populates the entity data with the current created entities.
// Parameters:
// string entityDataPath - The path of the entity information file.
//=============================================================================
void EntitySystem::initialize(std::string entityDataPath)
{
	if(!m_initialized)
	{
		m_settingsManager.open(entityDataPath, SettingIO::READ);
		
		if (m_settingsManager.isOpen())
		{
			m_entityDataPath = entityDataPath;
			loadEntityData();
			m_settingsManager.close();
			m_initialized = true;
		}
	}
}

//=============================================================================
// Function: int createEntity(entityKey)
// Description:
// Creates a new entity based on the information in the entity data.
// Parameters:
// entityKey entityName - The data ID of the entity to load.
// Output:
// int - The id of the entity that was created. 
// Returns -1 on failure.
//=============================================================================
int EntitySystem::createEntity(entityKey entityName)
{
	if (m_initialized)
	{
		// Make sure we can actually load an entity.
		if (m_settingsManager.isOpen())
		{
			m_settingsManager.close();
		}

		auto mit = m_entityData.find(entityName);

		if (mit != m_entityData.end())
		{
			int entityID = findNextAvailableID();

			m_settingsManager.open(mit->second, SettingIO::READ);

			if (m_settingsManager.isOpen())
			{
				if (entityType(entityID) == "")
				{
					std::string type = m_settingsManager.loadSetting("Type");

					if (type != "")
					{
						m_entityType.insert(std::make_pair(entityID, type));
					}
				}

				loadComponents(entityID, Vector2D(0,0));

				m_entityList[entityID] = true;
				return entityID;
			}
		}
	}

	return -1;
}

//=============================================================================
// Function: int createEntity(entityKey, Vector2D)
// Description:
// Creates a new entity based on the information in the entity data.
// Parameters:
// entityKey entityName - The data ID of the entity to load.
// Vector2D position - The position of the entity.
// Output:
// int - The id of the entity that was created. 
// Returns -1 on failure.
//=============================================================================
int EntitySystem::createEntity(entityKey entityName, Vector2D position)
{
	if (m_initialized)
	{
		// Make sure we can actually load an entity.
		if (m_settingsManager.isOpen())
		{
			m_settingsManager.close();
		}

		auto mit = m_entityData.find(entityName);

		if (mit != m_entityData.end())
		{
			int entityID = findNextAvailableID();

			m_settingsManager.open(mit->second, SettingIO::READ);

			if (m_settingsManager.isOpen())
			{
				if(entityType(entityID) == "")
				{
					std::string type = m_settingsManager.loadSetting("Type");

					if (type != "")
					{
						m_entityType.insert(std::make_pair(entityID, type));
					}
				}

				loadComponents(entityID, position);

				m_entityList[entityID] = true;
				return entityID;
			}
		}
	}

	return -1;
}

//=============================================================================
// Function: string entityType(int)
// Description:
// Gets the entity type of the entity related to the ID.
// Parameters:
// int entityID - The ID to get the type of.
// Output:
// string - A string containing the type of the entity.
// Returns "" on failure.
//=============================================================================
std::string EntitySystem::entityType(int entityID)
{
	std::string type = "";

	if(m_entityList[entityID] == true)
	{
		auto mit = m_entityType.find(entityID);

		if (mit != m_entityType.end())
		{
			type = mit->second;
		}
	}

	return type;
}

//=============================================================================
// Function: AttackInfo* entityAttack(int)
// Description:
// Gets the attack information for the specific entity.
// Parameters:
// int entityID - The entity to get attack information for.
// Output:
// AttackInfo* - The attack information retrieved.
// Returns NULL if there is no attack information for the entity.
//=============================================================================
AttackInfo* EntitySystem::entityAttack(int entityID)
{
	AttackInfo *attack = NULL;
	std::string type = entityType(entityID);

	if(type != "")
	{
		auto mit = m_entityAttacks.find(type);

		if(mit != m_entityAttacks.end())
		{
			attack = mit->second;
		}
	}

	return attack;
}

//=============================================================================
// Function: int findNextAvaliableID()
// Description:
// Looks through the current IDs and finds the first one that hasn't
// been initialized. If there isn't one currently available, then it adds one.
// Output:
// int - The ID number that is available.
//=============================================================================
int EntitySystem::findNextAvailableID()
{
	auto mit = m_entityList.begin();
	int ID = 0;
	bool idFound = false;

	while (mit != m_entityList.end() && !idFound)
	{
		if (mit->second == false)
		{
			idFound = true;
		}
		else
		{
			ID++;
		}

		mit++;
	}

	return ID;
}

//=============================================================================
// Function: void loadComponents(int, Vector2D)
// Description:
// Loads the components for the entity based off of the entity data.
// Parameters:
// int entityID - The entity ID to create components for.
// Vector2D position - The position of the entity.
//=============================================================================
void EntitySystem::loadComponents(int entityID, Vector2D position)
{
	loadPhysicsComponents(entityID, position);
	loadRenderComponents(entityID, position);
}

//=============================================================================
// Function: void loadPhysicsComponents(int, Vector2D)
// Description:
// Loads all of the physics components for the entity.
// Parameters:
// int entityID - The entity to load components for.
// Vector2D position - The position of the entity.
//=============================================================================
void EntitySystem::loadPhysicsComponents(int entityID, Vector2D position)
{
	if(m_settingsManager.isOpen())
	{
		PhysicsSystem *phys = PhysicsSystem::instance();
		VelocityComponent *vel = phys->getVelocityComponent(entityID);

		if (m_settingsManager.settingExists("CollisionComponent"))
		{
			Shape::ShapeType shape = Shape::NONE;

			std::string shapeSetting = "";

			if(m_settingsManager.settingExists("Shape"))
			{
				shapeSetting = m_settingsManager.loadSetting("Shape");

				if (shapeSetting == "Rectangle")
				{
					shape = Shape::RECTANGLE;
				}
				else if(shapeSetting == "Circle")
				{
					shape = Shape::CIRCLE;
				}
			}

			CollisionComponent *collision = phys->createCollisionComponent(entityID, shape, 0, 0);

			if (collision)
			{
				collision->setCenter(position);

				// Setup the shape information
				if (shape == Shape::RECTANGLE)
				{
					pRectangle rect = dynamic_cast<pRectangle>(collision->shape());

					if (rect)
					{
						int width = 0;
						int height = 0;

						std::string strWidth = m_settingsManager.loadSetting("RectWidth");
						std::string strHeight = m_settingsManager.loadSetting("RectHeight");

						if (strWidth != "") { width = stoi(strWidth); }
						if (strHeight != "") { height = stoi(strHeight); }

						rect->setWidth(width);
						rect->setHeight(height);
					}
				}
				else if (shape == Shape::CIRCLE)
				{
					pCircle circle = dynamic_cast<pCircle>(collision->shape());

					int radius = 0;

					std::string strRadius = m_settingsManager.loadSetting("CircleRadius");

					if (strRadius != "") { radius = stoi(strRadius); }

					circle->setRadius(radius);
				}

				if (m_settingsManager.settingExists("Solid"))
				{
					if (m_settingsManager.loadSetting("Solid") == "1")
					{
						collision->setSolid(true);
					}
				}
			}
		}
	}
}

//=============================================================================
// Function: void loadRenderComponents(int, Vector2D)
// Description:
// Loads all of the render components for the specified entity.
// TODO: If there are too many components in the future, separate it into
// individual functions.
// Parameters:
// int entityID - The entity to load components for.
// Vector2D position - The position to load the entity at.
//=============================================================================
void EntitySystem::loadRenderComponents(int entityID, Vector2D position)
{
	RenderSystem *render = RenderSystem::instance();

	if(m_settingsManager.settingExists("SpriteComponent"))
	{
		std::string texturePath = m_settingsManager.loadSetting("SpriteTexture");

		if(texturePath != "")
		{
			SpriteComponent *sprite = render->createSprite(entityID, texturePath);

			if (sprite)
			{
				sprite->setPosition(position);

				if (m_settingsManager.settingExists("SpriteAnchorX") &&
					m_settingsManager.settingExists("SpriteAnchorY"))
				{
					int anchorX = 0;
					int anchorY = 0;

					std::string strAnchorX = m_settingsManager.loadSetting("SpriteAnchorX");
					std::string strAnchorY = m_settingsManager.loadSetting("SpriteAnchorY");

					if (strAnchorX != "") { anchorX = stoi(strAnchorX); }
					if (strAnchorY != "") { anchorY = stoi(strAnchorY); }

					Vector2D anchor((float)anchorX, (float)anchorY);

					sprite->setAnchor(anchor);
				}

				// Now load the animations.
				if (m_settingsManager.settingExists("AnimationComponent"))
				{
					AnimationComponent *animation = render->createAnimationComponent(entityID);

					if (animation)
					{
						std::string s_Count = m_settingsManager.loadSetting("Animation_Count");

						if (s_Count != "")
						{
							int animationCount = stoi(s_Count);

							int currentX = 0;
							int currentY = 0;
							int totalWidth = sprite->width();
							int totalHeight = sprite->height();

							for (int i = 0; i < animationCount; i++)
							{
								std::string currentAnimation = "Animation_" + std::to_string(i);
								std::string name = m_settingsManager.loadSetting(currentAnimation);

								if (name != "")
								{
									std::string s_width = m_settingsManager.loadSetting(name + "_Width");
									std::string s_height = m_settingsManager.loadSetting(name + "_Height");
									std::string s_frames = m_settingsManager.loadSetting(name + "_Frames");
									std::string s_loop = m_settingsManager.loadSetting(name + "_Loop");
									std::string s_speed = m_settingsManager.loadSetting(name + "_Speed");

									if (s_width != "" && s_height != "" && s_frames != "")
									{
										bool loop = true;

										if (s_loop == "")
										{
											loop = false;
										}
										else if (s_loop == "0")
										{
											loop = false;
										}

										float speed = 0.0f;

										if (s_speed != "")
										{
											speed = stof(s_speed);
										}

										int width = stoi(s_width);
										int height = stoi(s_height);
										int frames = stoi(s_frames);

										animation->addAnimation(name, loop, speed);

										for (int j = 0; j < frames; j++)
										{
											SDL_Rect rect{ currentX, currentY, width, height };

											animation->addFrame(name, rect);

											currentY += height;

											if (totalHeight <= currentY)
											{
												currentY = 0;
												currentX += width;
											}
										}

										if (i == 0)
										{
											animation->setAnimation(name);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// Function: void loadEntityData()
// Description:
// Loads in all of the entity data inside the data file.
//=============================================================================
void EntitySystem::loadEntityData()
{
	entityKey key = "";
	std::string entityData = "";
	int entityCount = m_settingsManager.settingCount();

	for(int i = 0; i < entityCount; i++)
	{
		key = std::to_string(i);

		entityData = m_settingsManager.loadSetting(key);
		
		if(entityData != "")
		{
			m_entityData.insert(std::make_pair(key, entityData));
		}
	}
}
