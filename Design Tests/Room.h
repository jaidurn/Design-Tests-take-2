#pragma once
//==========================================================================================
// File Name: Room.h
// Author: Brian Blackmon
// Date Created: 7/17/2019
// Purpose: 
// Holds the information about a room.
//==========================================================================================
#include <vector>
#include "Rectangle.h"
#include <memory>

class Room;

class Door
{
public:
	Door(Vector2D position)
		:m_position(position), connected(false), connection(NULL), connectedRoom(NULL), m_closed(true)
	{
	}

	Vector2D m_position;
	bool connected;
	Door *connection;
	Room *connectedRoom;
	bool m_closed;
};

class Room
{
public:
	Room(int x, int y, int width, int height, std::string map);
	~Room();

	Shape::Rectangle *rect() { return m_rect; }
	std::string map() { return m_map; }
	bool visible() { return m_visible; }

	void addTileType(int tileType, int x, int y);
	void getTileType(int x, int y);
	void setTileType(int tileType, int x, int y);

	void addConnection(Room *room);
	void removeConnection(int index);
	void replaceConnection(Room *room, int index);
	int connectionCount();
	Room* getConnection(int index);

	void show();
	void hide();

	void addDoor(Door *door)
	{
		m_doors.push_back(door);
	}

	void clearDoors()
	{
		for(unsigned int i = 0; i < m_doors.size(); i++)
		{
			m_doors[i] = NULL;
		}
	}

	Door* getDoor(int index)
	{
		if(0 <= index && (unsigned int)index < m_doors.size())
		{
			return m_doors[index];
		}

		return NULL;
	}

	int doorCount()
	{
		return (int)m_doors.size();
	}
	
	void addEntity(int entityID);
	void removeEntity(int entityID);

private:
	Shape::Rectangle *m_rect;

	std::vector<int> m_entities;
	std::vector<Room*> m_connections;
	std::vector<Door*> m_doors;
	std::string m_map;

	bool m_visible;

	void cleanUp();
};

