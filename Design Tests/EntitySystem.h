#pragma once
//==========================================================================================
// File Name: EntitySystem.h
// Author: Brian Blackmon
// Date Created: 5/2/2019
// Purpose: 
// Loads and keeps track of entity information.
//==========================================================================================
#include "SettingIO.h"
#include "Vector2D.h"
#include "AttackInfo.h"
#include <map>
#include <unordered_map>

typedef int entityKey;
class IMessage;

class EntitySystem
{
public:
	~EntitySystem();

	static EntitySystem* instance()
	{
		static EntitySystem *instance = new EntitySystem();
		return instance;
	}

	void initialize(std::string entityDataPath);

	int createEntity(entityKey key);
	int createEntity(entityKey key, Vector2D position);

	entityKey getEntityKey(int entityID);
	std::string entityType(int entityID);
	AttackInfo* entityAttack(entityKey key);

	void processMessage(IMessage *message);

private:
	EntitySystem()
		:m_currentEntityID(0), m_initialized(false)
	{
	}

	int m_currentEntityID;
	bool m_initialized;
	SettingIO m_settingsManager;
	std::string m_entityDataPath;

	std::map<entityKey, std::string> m_entityData;
	std::map<int, bool> m_entityList;
	std::map<int, std::string> m_entityType;
	std::map<int, entityKey> m_entityKeys;
	std::map<entityKey, AttackInfo*> m_entityAttacks;

	int findNextAvailableID();

	void loadComponents(int entityID, Vector2D position);
	void loadPhysicsComponents(int entityID, Vector2D position);
	void loadRenderComponents(int entityID, Vector2D position);
	void loadLogicComponents(int entityID);
	void loadAttackInfo(entityKey key);
	void loadEntityData();

	void deleteEntity(int entityID);
};