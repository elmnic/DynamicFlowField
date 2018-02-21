#pragma once

#include <string>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void loadEntities(std::string mapName);
};

