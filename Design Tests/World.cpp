#include "World.h"
#include <iostream>
#include "ResourceManager.h"
#include "Texture.h"
#include "EntitySystem.h"
#include "RenderSystem.h"
#include "EntityDestroyMessage.h"
#include "PhysicsSystem.h"
#include "LogicSystem.h"
#include "DoorLogicComponent.h"

World::World(std::string roomPath)
	:m_settingsLoader(), m_exists(false)
{
	m_settingsLoader.open(roomPath, SettingIO::READ);

	if(!m_settingsLoader.isOpen())
	{
		std::cout << "Error opening room data: '"
			<< roomPath
			<< "'!\n";
	}
	else
	{
		loadData(roomPath);
	}
}

World::~World()
{
	for(unsigned int i = 0; i < m_rooms.size(); i++)
	{
		delete m_rooms[i];
	}

	for(unsigned int i = 0; i < m_hallways.size(); i++)
	{
		delete m_hallways[i];
	}

	m_rooms.empty();
	m_hallways.empty();

	auto mit = m_roomData.begin();

	while(mit != m_roomData.end())
	{
		mit = m_roomData.erase(mit);

		if(mit != m_roomData.end())
		{
			mit++;
		}
	}
	
	m_exists = false;
}

Room* World::getRoom(int index)
{
	Room *room = NULL;

	if(0 <= index && index < (int)m_rooms.size())
	{
		room = m_rooms[index];
	}

	return room;
}

//=============================================================================
// Function: string roomData(int)
// Description:
// Gets the room data for the specified type.
// Parameters:
// int roomType - The type of room to get data for.
// Output:
// string - The path that contains the room data.
//=============================================================================
std::string World::roomData(int roomType)
{
	std::string data = "";

	auto mit = m_roomData.find(roomType);

	if(mit != m_roomData.end())
	{
		data = mit->second;
	}

	return data;
}

void World::dungeon()
{
	createDungeon();

	while(m_rooms.size() < 10)
	{
		clear();
		createDungeon();
	}

	generateHallways();

	for(unsigned int i = 0; i < m_rooms.size(); i++)
	{
		m_validRooms.insert(std::make_pair(m_rooms[i], false));
	}

	int runCount = 0;

	validateRooms(m_rooms[0], runCount);

	bool roomsValid = true;

	auto mit = m_validRooms.begin();

	while(mit != m_validRooms.end())
	{
		if(mit->second == false)
		{
			roomsValid = false;
			break;
		}

		mit++;
	}

	if (roomsValid)
	{
		loadTiles();
		m_exists = true;
	}
	else
	{
		std::cout << "BLEH\n";
		clear();
	}
}

void World::clear()
{
	for (unsigned int i = 0; i < m_rooms.size(); i++)
	{
		delete m_rooms[i];
	}

	m_rooms.resize(0);

	for(unsigned int i = 0; i < m_hallways.size(); i++)
	{
		m_hallways[i]->clearDoors();
		delete m_hallways[i];
	}

	m_hallways.resize(0);

	m_exists = false;

	m_validRooms.clear();
}

void World::renderRooms()
{
	for(unsigned int i = 0; i < m_rooms.size(); i++)
	{
		if(!m_rooms[i]->visible())
		{
			for(int j = 0; j < m_rooms[i]->doorCount(); j++)
			{
				if(!m_rooms[i]->getDoor(j)->m_closed)
				{
					m_rooms[i]->show();
					break;
				}
			}
		}
	}

	for(unsigned int i = 0; i < m_hallways.size(); i++)
	{
		if (!m_hallways[i]->visible())
		{
			for (int j = 0; j < m_hallways[i]->doorCount(); j++)
			{
				if (!m_hallways[i]->getDoor(j)->m_closed)
				{
					m_hallways[i]->show();
					break;
				}
			}
		}
	}
}

//=============================================================================
// Function: void loadData(string)
// Description:
// Loads the room data information into the room data map.
// Parameters:
// string roomPath - The path containing the location of the room data.
//=============================================================================
void World::loadData(std::string roomPath)
{
	if(m_settingsLoader.isOpen())
	{
		std::string sRoomCount = m_settingsLoader.loadSetting("RoomCount");

		if (sRoomCount != "")
		{
			int roomCount = std::stoi(m_settingsLoader.loadSetting("RoomCount"));

			for (int i = 0; i < roomCount; i++)
			{
				std::string roomName = "Room" + std::to_string(i);
				std::string data = m_settingsLoader.loadSetting(roomName);

				if (data != "")
				{
					m_roomData.insert(std::make_pair(i, data));
				}
			}
		}
		else
		{
			std::cout << "Error loading room data '"
				<< roomPath
				<< "'!\n";
		}
	}
}

//=============================================================================
// Function: Room* generateRoom(int, int, int, int)
// Description:
// Generates the tiles for a room.
// Parameters:
// int x - The x position of the room.
// int y - The y position of the room.
// int width - The width of the room.
// int height - The height of the room.
// Output:
// Returns the created room on success.
// Returns NULL on failure.
//=============================================================================
void World::loadTiles()
{
	for (unsigned int room = 0; room < m_rooms.size(); room++)
	{
		int x = (int)m_rooms[room]->rect()->getTopLeft().getX();
		int y = (int)m_rooms[room]->rect()->getTopLeft().getY();
		int width = m_rooms[room]->rect()->width();
		int height = m_rooms[room]->rect()->height();

		int tileX = x;
		int tileY = y;
		int tileType = 0;
		int tileSize = 32;
		int tile = 0;

		Texture* texture = ResourceManager::instance()->getTexture("Resources/tilesetA.png");

		std::string texturePath = "Resources/tilesetA.png";

		int tileCount = texture->height() / tileSize;

		EntitySystem *entity = EntitySystem::instance();

		// generate walls.
		std::fstream mapStream;

		mapStream.open(m_rooms[room]->map(), std::ios::in);

		if (mapStream.is_open())
		{
			int tileType = 0;
			bool tileSolid = false;

			while (mapStream >> tileType)
			{
				if (0 <= tileType && tileType < tileCount)
				{
					mapStream >> tileSolid;

					for (int i = 0; i < m_rooms[room]->doorCount(); i++)
					{
						if (m_rooms[room]->getDoor(i)->connected)
						{
							Door *door = m_rooms[room]->getDoor(i);

							int doorX = (int)(m_rooms[room]->rect()->center().getX() - (m_rooms[room]->rect()->width() / 2));
							int doorY = (int)(m_rooms[room]->rect()->center().getY() - (m_rooms[room]->rect()->height() / 2));

							doorX += (int)m_rooms[room]->getDoor(i)->m_position.getX();
							doorY += (int)m_rooms[room]->getDoor(i)->m_position.getY();

							if (doorX == (x + width))
							{
								doorX -= tileSize;
							}

							if (doorY == (y + height))
							{
								doorY -= tileSize;
							}

							if (door->m_position.getX() == 0 || door->m_position.getX() == m_rooms[room]->rect()->width())
							{
								if(tileX == doorX && tileY == doorY + tileSize)
								{
									tileType = 0;
									tileSolid = 0;
								}
								
								if (door->m_position.getX() == 0)
								{
									if (tileX == doorX && tileY == doorY + (tileSize * 2))
									{
										tileType = 13;
										tileSolid = 1;
									}
									else if (tileX == doorX && tileY == doorY - tileSize)
									{
										tileType = 10;
										tileSolid = 1;
									}
								}
								else
								{
									if(tileX == doorX && tileY == doorY + (tileSize * 2))
									{
										tileType = 12;
										tileSolid = 1;
									}
									else if(tileX == doorX && tileY == doorY - tileSize)
									{
										tileType = 11;
										tileSolid = 1;
									}
								}
							}
							else if(door->m_position.getY() == 0 || door->m_position.getY() == m_rooms[room]->rect()->height())
							{
								if(tileX == doorX + tileSize && tileY == doorY)
								{
									tileType = 0; 
									tileSolid = 0;
								}

								if(door->m_position.getY() == 0)
								{
									if(tileX == doorX - tileSize && tileY == doorY)
									{
										tileType = 10;
										tileSolid = 1;
									}
									else if(tileX == doorX + (tileSize * 2) && tileY == doorY)
									{
										tileType = 11;
										tileSolid = 1;
									}
								}
								else
								{
									if (tileX == doorX - tileSize && tileY == doorY)
									{
										tileType = 13;
										tileSolid = 1;
									}
									else if (tileX == doorX + (tileSize * 2) && tileY == doorY)
									{
										tileType = 12;
										tileSolid = 1;
									}
								}
							}

							if (tileX == doorX && tileY == doorY)
							{
								tileType = 0;
								tileSolid = 0;
							}
						}
					}

					int entityType = 4;

					if (tileSolid)
					{
						entityType = 3;
					}

					tile = entity->createEntity(entityType, Vector2D((float)tileX + (tileSize / 2), (float)tileY + (tileSize / 2)));

					int randTex = 0;

					if (tileType < 2)
					{
						randTex = rand() % 5;
					}

					SpriteComponent *sprite = RenderSystem::instance()->createSprite(tile, texturePath, Vector2D((float)(tileX + (tileSize / 2)), (float)(tileY + (tileSize / 2))));

					if (sprite)
					{
						sprite->setClip(randTex * tileSize, tileType * tileSize, tileSize, tileSize);
					}

					m_rooms[room]->addEntity(tile);
				}

				tileX += tileSize;

				if ((x + width) <= tileX)
				{
					tileX = x;
					tileY += tileSize;
				}
			}
		}
	}

	for (unsigned int i = 0; i < m_hallways.size(); i++)
	{
		Door *doorA = m_hallways[i]->getDoor(0);
		Door *doorB = m_hallways[i]->getDoor(1);

		int hallwaySize = 32 * 3;
		int tileSize = 32;
		int tileCount = ((m_hallways[i]->rect()->width() / tileSize) * (m_hallways[i]->rect()->height() / tileSize));
		int startingX = (int)(m_hallways[i]->rect()->center().getX() - (m_hallways[i]->rect()->width() / 2));
		int startingY = (int)(m_hallways[i]->rect()->center().getY() - (m_hallways[i]->rect()->height() / 2));

		int tileX = startingX;
		int tileY = startingY;

		int tileType = 0;

		int tile = 0;

		bool solid = false;

		std::string texturePath = "Resources/tilesetA.png";

		EntitySystem *entity = EntitySystem::instance();

		if (doorA && doorB)
		{
			if ((doorA->m_position.getY() == 0) || 
				(doorB->m_position.getY() == 0))
			{
				for (int j = 0; j < tileCount; j++)
				{
					if (tileX == startingX)
					{
						tileType = 3;
					}
					else if (tileX == (startingX + m_hallways[i]->rect()->width() - tileSize))
					{
						tileType = 4;
					}
					else
					{
						tileType = 0;
					}

					if (tileType < 2)
					{
						solid = false;
					}
					else
					{
						solid = true;
					}

					if (solid)
					{
						tile = entity->createEntity(3, Vector2D((float)tileX + (tileSize / 2), (float)tileY + (tileSize / 2)));
					}
					else
					{
						tile = entity->createEntity(4, Vector2D((float)tileX + (tileSize / 2), (float)tileY + (tileSize / 2)));
					}

					int randTex = 0;

					if (tileType < 2)
					{
						randTex = rand() % 5;
					}

					SpriteComponent *sprite = RenderSystem::instance()->createSprite(tile, texturePath, Vector2D((float)(tileX + (tileSize / 2)), (float)(tileY + (tileSize / 2))));

					if (sprite)
					{
						sprite->setClip(randTex * tileSize, tileType * tileSize, tileSize, tileSize);
					}

					m_hallways[i]->addEntity(tile);

					tileX += tileSize;

					if (startingX + m_hallways[i]->rect()->width() <= tileX)
					{
						tileX = startingX;
						tileY += tileSize;
					}
				}
			}
			else if ((doorA->m_position.getX() == 0) ||
				(doorB->m_position.getX() == 0))
			{
				for (int j = 0; j < tileCount; j++)
				{
					if (tileY == startingY)
					{
						tileType = 2;
					}
					else if (tileY == (startingY + m_hallways[i]->rect()->height() - tileSize))
					{
						tileType = 5;
					}
					else
					{
						tileType = 0;
					}

					if (tileType < 2)
					{
						solid = false;
					}
					else
					{
						solid = true;
					}

					if (solid)
					{
						tile = entity->createEntity(3, Vector2D((float)tileX + (tileSize / 2), (float)tileY + (tileSize / 2)));
					}
					else
					{
						tile = entity->createEntity(4, Vector2D((float)tileX + (tileSize / 2), (float)tileY + (tileSize / 2)));
					}

					int randTex = 0;

					if (tileType < 2)
					{
						randTex = rand() % 5;
					}

					SpriteComponent *sprite = RenderSystem::instance()->createSprite(tile, texturePath, Vector2D((float)(tileX + (tileSize / 2)), (float)(tileY + (tileSize / 2))));

					if (sprite)
					{
						sprite->setClip(randTex * tileSize, tileType * tileSize, tileSize, tileSize);
					}

					m_hallways[i]->addEntity(tile);

					tileX += tileSize;

					if (startingX + m_hallways[i]->rect()->width() <= tileX)
					{
						tileX = startingX;
						tileY += tileSize;
					}
				}
			}
		}

		m_hallways[i]->hide();
	}

	// Create door entities
	for(unsigned int i = 0; i < m_rooms.size(); i++)
	{
		for(int j = 0; j < m_rooms[i]->doorCount(); j++)
		{
			Door *door = m_rooms[i]->getDoor(j);

			if (door->connected)
			{
				int entityType = 0;
				int doorX = 0;
				int doorY = 0; 
				int tileSize = 32;

				if(door->m_position.getX() == 0 || door->m_position.getX() == m_rooms[i]->rect()->width())
				{
					entityType = 5;

					if(door->m_position.getX() == 0)
					{
						doorX = (int)m_rooms[i]->rect()->getTopLeft().getX();
					}
					else if(door->m_position.getX() == m_rooms[i]->rect()->width())
					{
						doorX = (int)m_rooms[i]->rect()->getTopRight().getX();
					}

					doorY = (int)m_rooms[i]->rect()->getTopLeft().getY() + (int)door->m_position.getY() + tileSize;
				}
				else if (door->m_position.getY() == 0 || door->m_position.getY() == m_rooms[i]->rect()->height())
				{
					entityType = 6;

					if(door->m_position.getY() == 0)
					{
						doorY = (int)m_rooms[i]->rect()->getTopLeft().getY();
					}
					else if(door->m_position.getY() == m_rooms[i]->rect()->height())
					{
						doorY = (int)m_rooms[i]->rect()->getBottomLeft().getY();
					}

					doorX = (int)m_rooms[i]->rect()->getTopLeft().getX() + (int)door->m_position.getX() + tileSize;
				}

				if(doorX != 0 || doorY != 0)
				{
					int doorID = EntitySystem::instance()->createEntity(entityType, Vector2D((float)doorX, (float)doorY));

					LogicComponent *logic = LogicSystem::instance()->createLogicComponent(doorID, LogicComponent::LOGIC_DOOR);

					if(logic)
					{
						DoorLogicComponent *doorLogic = static_cast<DoorLogicComponent*>(logic);

						doorLogic->setDoor(door);
					}

					RenderSystem::instance()->setSpriteLayer(doorID, RenderSystem::RENDER_FOREGROUND1);

					m_rooms[i]->addEntity(doorID);

					for(unsigned int j = 0; j < m_hallways.size(); j++)
					{
						if(m_hallways[j]->getDoor(0) == door || m_hallways[j]->getDoor(1) == door)
						{
							m_hallways[j]->addEntity(doorID);
						}
					}
				}
			}
		}

		if (i != 0)
		{
			m_rooms[i]->hide();
		}
	}
}

void World::generateHallways()
{
	// Find the closest room for each room
	std::cout << "Generating hallways!\n";

	CollisionSystem *collisionSys = PhysicsSystem::instance()->collisionSystem();

	for (unsigned int i = 0; i < m_rooms.size(); i++)
	{
		for(int j = 0; j < m_rooms[i]->doorCount(); j++)
		{
			Door *doorA = m_rooms[i]->getDoor(j);

			if(doorA->connected)
			{
				Door *doorB = doorA->connection;

				Vector2D doorAPos = doorA->m_position + m_rooms[i]->rect()->getTopLeft();
				Vector2D doorBPos = doorB->m_position + doorA->connectedRoom->rect()->getTopLeft();

				Room *hallway = NULL;

				float hallwayX = 0;
				float hallwayY = 0;
				int hallwayWidth = 0;
				int hallwayHeight = 0;
				int hallwaySize = 96 + 32;

				bool clearDoors = false;

				if (isDoorValid(doorA, m_rooms[i], doorB, doorA->connectedRoom))
				{
					if (doorA->m_position.getX() == 0)
					{
						if (doorB->m_position.getX() == doorA->connectedRoom->rect()->width())
						{
							hallwayX = doorBPos.getX();
							hallwayWidth = (int)(doorAPos.getX() - hallwayX);
						}
					}
					else if (doorA->m_position.getX() == m_rooms[i]->rect()->width())
					{
						if (doorB->m_position.getX() == 0)
						{
							hallwayX = doorAPos.getX();
							hallwayWidth = (int)(doorBPos.getX() - hallwayX);
						}
					}
					else if (doorAPos.getX() == doorBPos.getX())
					{
						hallwayX = doorAPos.getX() - 32;
						hallwayWidth = hallwaySize;
					}

					if (doorA->m_position.getY() == 0)
					{
						if (doorB->m_position.getY() == doorA->connectedRoom->rect()->height())
						{
							hallwayY = doorBPos.getY();
							hallwayHeight = (int)(doorAPos.getY() - hallwayY);
						}
					}
					else if (doorA->m_position.getY() == m_rooms[i]->rect()->height())
					{
						if (doorB->m_position.getY() == 0)
						{
							hallwayY = doorAPos.getY();
							hallwayHeight = (int)(doorBPos.getY() - hallwayY);
						}
					}
					else if (doorAPos.getY() == doorBPos.getY())
					{
						hallwayY = doorAPos.getY() - 32;
						hallwayHeight = hallwaySize;
					}

					if (0 < hallwayWidth && 0 < hallwayHeight)
					{
						hallway = new Room((int)hallwayX, (int)hallwayY, hallwayWidth, hallwayHeight, "");

						bool validHallway = true;

						for (unsigned int k = 0; k < m_rooms.size(); k++)
						{
							if (collisionSys->rectInsideRect(hallway->rect(), m_rooms[k]->rect()) || collisionSys->rectInsideRect(m_rooms[k]->rect(), hallway->rect()))
							{
								clearDoors = true;
								validHallway = false;
								break;
							}
						}

						if (validHallway)
						{
							for (unsigned int k = 0; k < m_hallways.size(); k++)
							{
								if (hallway->rect()->center().getX() != m_hallways[k]->rect()->center().getX() ||
									hallway->rect()->center().getY() != m_hallways[k]->rect()->center().getY() ||
									hallway->rect()->width() != m_hallways[k]->rect()->width() ||
									hallway->rect()->height() != m_hallways[k]->rect()->height())
								{
									if (collisionSys->rectInsideRect(hallway->rect(), m_hallways[k]->rect()))
									{
										clearDoors = true;
										validHallway = false;
										break;
									}
								}
								else
								{
									validHallway = false;
									break;
								}
							}
						}

						if (!validHallway)
						{
							delete hallway;
							hallway = NULL;
						}
						else
						{
							m_hallways.push_back(hallway);
						}
					}
					else
					{
						hallway = new Room((int)hallwayX, (int)hallwayY, hallwayWidth, hallwayHeight, "");

						for (unsigned int k = 0; k < m_hallways.size(); k++)
						{
							if (hallway->rect()->center().getX() != m_hallways[k]->rect()->center().getX() ||
								hallway->rect()->center().getY() != m_hallways[k]->rect()->center().getY() ||
								hallway->rect()->width() != m_hallways[k]->rect()->width() ||
								hallway->rect()->height() != m_hallways[k]->rect()->height())
							{
								delete hallway;
								hallway = NULL;
								break;
							}
						}
					}
				}
				else
				{
					clearDoors = true;
				}
				
				if(!hallway && clearDoors)
				{
					doorA->connected = false;
					doorA->connection = NULL;
					doorA->connectedRoom = NULL;

					doorB->connected = false;
					doorB->connection = NULL;
					doorB->connectedRoom = NULL;
				}
				else if(hallway)
				{
					hallway->addDoor(doorA);
					hallway->addDoor(doorB);
				}
			}

		}
	}
}

void World::createDungeon()
{
	int xSize = 2000;
	int ySize = 2000;
	int maxRooms = 15;
	int minRooms = 25;
	int maxAttempts = 500;
	int maxLargeRooms = 3;
	int maxMediumRooms = 4;

	int currentLargeRooms = 0;
	int currentMediumRooms = 0;
	int currentRooms = 0;
	int currentAttempts = 0;

	int roomX = 0;
	int roomY = 0;
	int roomType = 0;
	int roomWidth = 0;
	int roomHeight = 0;

	Uint32 seed = SDL_GetTicks();

	srand(seed);

	CollisionSystem *collisionSys = PhysicsSystem::instance()->collisionSystem();

	roomType = rand() % m_roomData.size();

	m_settingsLoader.close();
	m_settingsLoader.open(roomData(roomType), SettingIO::READ);

	if (m_settingsLoader.isOpen())
	{
		std::string sWidth = m_settingsLoader.loadSetting("Width");
		std::string sHeight = m_settingsLoader.loadSetting("Height");
		std::string map = m_settingsLoader.loadSetting("Map");
		std::string sDoorCount = m_settingsLoader.loadSetting("DoorCount");

		if (sWidth != "" && sHeight != "" && map != "" && sDoorCount != "")
		{
			roomWidth = std::stoi(sWidth);
			roomHeight = std::stoi(sHeight);

			roomX = xSize / 2 - (roomWidth / 2);
			roomY = ySize / 2 - (roomHeight / 2);

			Room *room = new Room(roomX, roomY, roomWidth, roomHeight, map);

			int doorCount = std::stoi(sDoorCount);

			if (0 < doorCount)
			{
				for (int i = 0; i < doorCount; i++)
				{
					std::string doorName = "Door" + std::to_string(i);
					std::string sX = m_settingsLoader.loadSetting(doorName + "_x");
					std::string sY = m_settingsLoader.loadSetting(doorName + "_y");

					if (sX != "" && sY != "")
					{
						float doorX = std::stof(sX);
						float doorY = std::stof(sY);

						Door *door = new Door(Vector2D(doorX, doorY));

						room->addDoor(door);
					}
				}
			}

			m_rooms.push_back(room);

			while ((int)m_rooms.size() < maxRooms && currentAttempts < maxAttempts)
			{
				int roomCount = (int)m_rooms.size();

				while (0 < roomCount && !generateRooms(m_rooms[roomCount - 1], xSize, ySize))
				{
					roomCount--;
					currentAttempts++;
				}
			}
		}
	}
}

bool World::validateRooms(Room *room, int &roomsVisited)
{
	bool roomValid = false;

	auto mit = m_validRooms.find(room);

	if (mit->second != true)
	{
		if (roomsVisited < (int)m_rooms.size())
		{
			m_validRooms.find(room)->second = true;
			roomsVisited++;

			for(int i = 0; i < room->doorCount(); i++)
			{
				if(room->getDoor(i)->connected)
				{
					validateRooms(room->getDoor(i)->connectedRoom, roomsVisited);
				}
			}
		}
		else
		{
			roomValid = true;
		}
	}

	return roomValid;
}

bool World::generateRooms(Room *room, int maxX, int maxY)
{
	bool generated = false;
	if (room)
	{
		int doorsTried = 0;
		int doorCurrent = rand() % room->doorCount();

		int roomX = 0;
		int roomY = 0;
		int roomWidth = 0;
		int roomHeight = 0;
		int roomType = 0;

		std::string sWidth = "";
		std::string sHeight = "";
		std::string map = "";
		std::string sDoorCount = "";

		Room *next = NULL;

		CollisionSystem *collisionSys = PhysicsSystem::instance()->collisionSystem();

		while (!next && doorsTried < room->doorCount())
		{
			roomType = rand() % m_roomData.size();

			while (room->getDoor(doorCurrent)->connected == true && doorsTried < room->doorCount())
			{
				doorCurrent++;
				doorsTried++;

				if (doorCurrent == room->doorCount())
				{
					doorCurrent = 0;
				}
			}

			if (doorsTried == room->doorCount())
			{
				break;
			}

			Door *current = room->getDoor(doorCurrent);

			m_settingsLoader.close();
			m_settingsLoader.open(roomData(roomType), SettingIO::READ);

			if (m_settingsLoader.isOpen())
			{
				// Load the new room
				sWidth = m_settingsLoader.loadSetting("Width");
				sHeight = m_settingsLoader.loadSetting("Height");
				map = m_settingsLoader.loadSetting("Map");
				sDoorCount = m_settingsLoader.loadSetting("DoorCount");

				if (sWidth != "" && sHeight != "" && map != "" && sDoorCount != "")
				{
					roomWidth = std::stoi(sWidth);
					roomHeight = std::stoi(sHeight);
					int doorCount = std::stoi(sDoorCount);
					int xDiff = 0;
					int yDiff = 0;
					int tileSize = 32;

					//Get the right place for the room.
					if(room->getDoor(doorCurrent)->m_position.getX() == 0)
					{
						xDiff = -(tileSize * 2 + (room->rect()->width() / 2) + roomWidth);
					}
					else if(room->getDoor(doorCurrent)->m_position.getX() == room->rect()->width())
					{
						xDiff = (tileSize * 2 + (room->rect()->width() / 2));
					}
					else if(room->getDoor(doorCurrent)->m_position.getY() == 0)
					{
						yDiff = -(tileSize * 2 + (room->rect()->height() / 2) + roomHeight);
					}
					else if(room->getDoor(doorCurrent)->m_position.getY() == room->rect()->height())
					{
						yDiff = (tileSize * 2 + (room->rect()->height() / 2));
					}

					xDiff = xDiff + (int)room->rect()->center().getX();
					yDiff = yDiff + (int)room->rect()->center().getY();

					next = new Room(xDiff, yDiff, roomWidth, roomHeight, map);

					// Add the doors to the room
					for(int i = 0; i < doorCount; i++)
					{
						std::string doorName = "Door" + std::to_string(i);
						std::string sDoorX = m_settingsLoader.loadSetting(doorName + "_x");
						std::string sDoorY = m_settingsLoader.loadSetting(doorName + "_y");

						float doorX = 0;
						float doorY = 0;

						if (sDoorX != "" && sDoorY != "")
						{
							doorX = std::stof(sDoorX);
							doorY = std::stof(sDoorY);

							Door *door = new Door(Vector2D(doorX, doorY));

							next->addDoor(door);
						}
					}

					std::vector<Door*> validDoors;

					Door *current = room->getDoor(doorCurrent);

					// Find all the doors in the next room
					// capable of connecting to the current door
					for(int i = 0; i < next->doorCount(); i++)
					{
						Door *doorB = next->getDoor(i);

						if(isDoorValid(current, room, doorB, next))
						{
							validDoors.push_back(doorB);
						}
					}

					Door *valid = NULL;

					// If there are enough valid doors
					if(validDoors.size() != 0)
					{
						// Connect the rooms together.
						int randDoor = rand() % (int)validDoors.size();

						valid = validDoors[randDoor];

						Vector2D doorPosA = room->rect()->getTopLeft() + current->m_position;
						Vector2D doorPosB = next->rect()->getTopLeft() + validDoors[randDoor]->m_position;

						float newX = 0;
						float newY = 0;

						if((current->m_position.getX() == 0 && valid->m_position.getX() == next->rect()->width()) ||
							(current->m_position.getX() == room->rect()->width() && valid->m_position.getX() == 0))
						{
							newY = doorPosA.getY() - doorPosB.getY();
						}
						else if((current->m_position.getY() == 0 && valid->m_position.getY() == next->rect()->height()) ||
								(current->m_position.getY() == room->rect()->height() && valid->m_position.getY() == 0))
						{
							newX = doorPosA.getX() - doorPosB.getX();
						}

						newX = newX + next->rect()->center().getX();
						newY = newY + next->rect()->center().getY();

						// Make sure the room isn't out of bounds.
						if (0 <= (newX - (next->rect()->width() / 2)) && 
							0 <= (newY - (next->rect()->height() / 2)) && 
							(newX + (next->rect()->width() / 2)) < maxX && 
							(newY + (next->rect()->height() / 2)) < maxY)
						{
							next->rect()->setCenter(newX, newY);

							// Make sure the room doesn't collide with existing rooms
							for (unsigned int i = 0; i < m_rooms.size(); i++)
							{
								if (collisionSys->rectInsideRect(next->rect(), m_rooms[i]->rect()))
								{
									delete next;
									next = NULL;
									break;
								}
							}

							// Connect the doors
							if (next)
							{
								valid->connected = true;
								valid->connection = current;
								valid->connectedRoom = room;

								current->connected = true;
								current->connection = validDoors[randDoor];
								current->connectedRoom = next;
							}
						}
						else
						{
							delete next;
							next = NULL;
						}
					}
					else
					{
						delete next;
						next = NULL;
					}

					if (next && valid)
					{
						for (int i = 0; i < next->doorCount(); i++)
						{
							bool madeConnection = false;

							Door *nextDoor = next->getDoor(i);

							if (!nextDoor->connected && valid != nextDoor)
							{
								for (unsigned int j = 0; j < m_rooms.size(); j++)
								{
									if (next != m_rooms[j])
									{
										for (int k = 0; k < m_rooms[j]->doorCount(); k++)
										{
											Door *otherDoor = m_rooms[j]->getDoor(k);

											if (!otherDoor->connected)
											{
												if (isDoorValid(nextDoor, next, otherDoor, m_rooms[j]))
												{
													if (valid->m_position.getX() == 0 || valid->m_position.getX() == next->rect()->width())
													{
														Vector2D nextDoorPos = next->rect()->getTopLeft() + nextDoor->m_position;
														Vector2D otherDoorPos = m_rooms[j]->rect()->getTopLeft() + otherDoor->m_position;

														float oldRoomX = next->rect()->center().getX();

														float newRoomX = otherDoorPos.getX() - nextDoorPos.getX() + oldRoomX;

														if (0 <= (newRoomX - (next->rect()->width() / 2)) &&
															(newRoomX + (next->rect()->width() / 2)) < maxX)
														{
															//Attemp to move the room.
															next->rect()->setCenterX(newRoomX);

															bool rectFit = true;

															nextDoorPos = next->rect()->getTopLeft() + nextDoor->m_position;

															if (nextDoor->m_position.getX() == 0)
															{
																if (nextDoorPos.getX() <= otherDoorPos.getX())
																{
																	next->rect()->setCenterX(oldRoomX);
																	rectFit = false;
																}
															}
															else if (nextDoor->m_position.getX() == next->rect()->width())
															{
																if (otherDoorPos.getX() <= nextDoorPos.getX())
																{
																	next->rect()->setCenterX(oldRoomX);
																	rectFit = false;
																}
															}

															if (rectFit)
															{
																//Check if it collides
																for (unsigned int l = 0; l < m_rooms.size(); l++)
																{
																	if (next != m_rooms[l])
																	{
																		if (collisionSys->rectInsideRect(next->rect(), m_rooms[l]->rect()))
																		{
																			next->rect()->setCenterX(oldRoomX);
																			rectFit = false;
																			break;
																		}
																	}
																}
															}

															if (rectFit)
															{
																nextDoor->connected = true;
																nextDoor->connection = otherDoor;
																nextDoor->connectedRoom = m_rooms[j];

																otherDoor->connected = true;
																otherDoor->connection = nextDoor;
																otherDoor->connectedRoom = next;

																madeConnection = true;
															}
														}
														//Find the amount to move to connect
													}
													else if (valid->m_position.getY() == 0 || valid->m_position.getY() == next->rect()->height())
													{
														Vector2D nextDoorPos = next->rect()->getTopLeft() + nextDoor->m_position;
														Vector2D otherDoorPos = m_rooms[j]->rect()->getTopLeft() + otherDoor->m_position;

														float oldRoomY = next->rect()->center().getY();

														float newRoomY = otherDoorPos.getY() - nextDoorPos.getY() + oldRoomY;

														if (0 <= (newRoomY - (next->rect()->height() / 2)) &&
															(newRoomY + (next->rect()->height() / 2)) < maxY)
														{
															//Attemp to move the room.

															next->rect()->setCenterY(newRoomY);

															bool rectFit = true;

															nextDoorPos = next->rect()->getTopLeft() + nextDoor->m_position;

															if (nextDoor->m_position.getY() == 0)
															{
																if (nextDoorPos.getY() <= otherDoorPos.getY())
																{
																	next->rect()->setCenterY(oldRoomY);
																	rectFit = false;
																}
															}
															else if (nextDoor->m_position.getY() == next->rect()->height())
															{
																if (otherDoorPos.getY() <= nextDoorPos.getY())
																{
																	next->rect()->setCenterY(oldRoomY);
																	rectFit = false;
																}
															}

															if (rectFit)
															{
																//Check if it collides
																for (unsigned int l = 0; l < m_rooms.size(); l++)
																{
																	if (next != m_rooms[l])
																	{
																		if (collisionSys->rectInsideRect(next->rect(), m_rooms[l]->rect()))
																		{
																			next->rect()->setCenterY(oldRoomY);
																			rectFit = false;
																			break;
																		}
																	}
																}
															}

															if (rectFit)
															{
																nextDoor->connected = true;
																nextDoor->connection = otherDoor;
																nextDoor->connectedRoom = m_rooms[j];

																otherDoor->connected = true;
																otherDoor->connection = nextDoor;
																otherDoor->connectedRoom = next;

																madeConnection = true;
															}
														}
													}
												}
											}

											if (madeConnection)
											{
												break;
											}
										}
									}

									if (madeConnection)
									{
										break;
									}
								}
							}

							if (madeConnection)
							{
								break;
							}
						}
					}
				}
			}

			doorCurrent++;

			if (doorCurrent == room->doorCount())
			{
				doorCurrent = 0;
			}

			doorsTried++;
		}

		if (next)
		{
			m_rooms.push_back(next);
			generated = true;
		}
	}
	
	return generated;
}

bool World::isDoorValid(Door *doorA, Room *roomA, Door *doorB, Room *roomB)
{
	bool validDoor = false;

	Vector2D doorAPos = roomA->rect()->getTopLeft() + doorA->m_position;
	Vector2D doorBPos = roomB->rect()->getTopLeft() + doorB->m_position;

	if(doorA->m_position.getX() == 0)
	{
		if (doorB->m_position.getX() == roomB->rect()->width())
		{
			if (doorBPos.getX() <= doorAPos.getX())
			{
				validDoor = true;
			}
		}
	}
	else if(doorA->m_position.getX() == roomA->rect()->width())
	{
		if(doorB->m_position.getX() == 0)
		{
			if (doorAPos.getX() <= doorBPos.getX())
			{
				validDoor = true;
			}
		}
	}
	else if(doorA->m_position.getY() == 0)
	{
		if(doorB->m_position.getY() == roomB->rect()->height())
		{
			if (doorBPos.getY() <= doorAPos.getY())
			{
				validDoor = true;
			}
		}
	}
	else if(doorA->m_position.getY() == roomA->rect()->height())
	{
		if(doorB->m_position.getY() == 0)
		{
			if (doorAPos.getY() <= doorBPos.getY())
			{
				validDoor = true;
			}
		}
	}

	return validDoor;
}