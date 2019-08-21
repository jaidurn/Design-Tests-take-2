#include "Room.h"
#include "MessageSystem.h"
#include "EntityDestroyMessage.h"
#include "RenderSystem.h"


Room::Room(int x, int y, int width, int height, std::string map)
	:m_rect(NULL), m_map(map), m_visible(true)
{
	m_rect = new Shape::Rectangle((float)(x + (width / 2)), (float)(y + (height / 2)), width, height);
}


Room::~Room()
{
	cleanUp();
}

void Room::addConnection(Room *room)
{
	if(room)
	{
		m_connections.push_back(room);
	}
}

void Room::removeConnection(int index)
{
	if (0 <= index && index < (int)m_connections.size())
	{
		auto it = std::next(m_connections.begin(), index);

		it = m_connections.erase(it);
	}
}

void Room::replaceConnection(Room *room, int index)
{
	if(0 <= index && index < (int)m_connections.size())
	{
		m_connections[index] = room;
	}
}

int Room::connectionCount()
{
	return (int)m_connections.size();
}

Room* Room::getConnection(int index)
{
	Room *room = NULL;

	if(0 <= index && index < (int)m_connections.size())
	{
		room = m_connections[index];
	}

	return room;
}

void Room::show()
{
	if (!m_visible)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			RenderSystem *sysRender = RenderSystem::instance();

			SpriteComponent *sprite = sysRender->getSprite(m_entities[i]);

			if (sprite)
			{
				sprite->setVisible(true);
			}
		}
		
		m_visible = true;
	}
}

void Room::hide()
{
	if (m_visible)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			RenderSystem *sysRender = RenderSystem::instance();

			SpriteComponent *sprite = sysRender->getSprite(m_entities[i]);

			if (sprite)
			{
				sprite->setVisible(false);
			}
		}

		m_visible = false;
	}
}

//=============================================================================
// Function: void addEntity(int)
// Description:
// Adds an entity to the room's entity list.
// Parameters:
// int entityID - The id of the entity to add.
//=============================================================================
void Room::addEntity(int entityID)
{
	m_entities.push_back(entityID);
}

//=============================================================================
// Function: void removeEntity(int)
// Description:
// Removes an entity from the room's entity list.
// Parameters:
// int entityID - The id of the entity to remove.
//=============================================================================
void Room::removeEntity(int entityID)
{
	auto vit = m_entities.begin();
	int index = 0;

	while(vit != m_entities.end())
	{
		if(m_entities[index] == entityID)
		{
			vit = m_entities.erase(vit);

			break;
		}

		vit++;
		index++;
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up the room by destroying all of the entities related to it.
//=============================================================================
void Room::cleanUp()
{
	for(unsigned int i = 0; i < m_entities.size(); i++)
	{
		EntityDestroyMessage *destroy = new EntityDestroyMessage(m_entities[i]);

		MessageSystem::instance()->pushMessage(destroy);
	}

	m_entities.clear();

	for(unsigned int i = 0; i < m_doors.size(); i++)
	{
		delete m_doors[i];
		m_doors[i] = NULL;
	}

	m_doors.clear();

	delete m_rect; 
	m_rect = NULL;
}

