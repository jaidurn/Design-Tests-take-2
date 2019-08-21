#pragma once
//==========================================================================================
// File Name: World.h
// Author: Brian Blackmon
// Date Created: 7/17/2019
// Purpose: 
// Holds the information about the world. Can create rooms.
//==========================================================================================
#include "Room.h"
#include "SettingIO.h"
#include <string>
#include <vector>
#include <map>

class World
{
public:
	World(std::string roomPath);
	~World();

	Room* getRoom(int index);
	std::string roomData(int roomType);
	int getRoomCount() { return (int)m_rooms.size(); }
	
	bool exists() { return m_exists; }
	
	void dungeon();
	void clear();
	void renderRooms();

private:
	SettingIO m_settingsLoader;
	std::vector<Room*>m_rooms;
	std::vector<Room*>m_hallways;
	std::map<int, std::string> m_roomData;
	std::map<Room*, bool> m_validRooms;
	bool m_exists;

	void loadData(std::string roomPath);
	void loadTiles();
	void generateHallways();

	bool validateRooms(Room *room, int &runCount);

	bool generateRooms(Room *room, int maxX, int maxY);
	bool isDoorValid(Door *doorA, Room *roomA, Door *doorB, Room *roomB);

	void createDungeon();
};

