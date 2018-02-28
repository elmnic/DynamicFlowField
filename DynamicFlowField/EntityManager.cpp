#include "EntityManager.h"


EntityManager* EntityManager::instance()
{
	static EntityManager instance;
	return &instance;
}


EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

void EntityManager::createBuilding(int size, Toolbox::BuildingType type, sf::Vector2i pos)
{
	mBuildingVector.push_back(new Building(size, type, pos));
}

void EntityManager::createAgent(sf::Vector2i startPos)
{
	//TODO:
}

void EntityManager::update()
{
}

void EntityManager::render(sf::RenderWindow & window)
{
	for (auto it : mBuildingVector)
		it->render(window);

	for (auto it : mAgentVector)
		it->render(window);
}
