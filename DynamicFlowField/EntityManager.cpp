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
	for (auto it : mBuildings)
		delete it;

	for (auto it : mRegions)
		delete it;

	mBuildings.clear();
	mRegions.clear();
}

void EntityManager::createBuilding(int size, Toolbox::BuildingType type, sf::Vector2i pos)
{
	Building *building = new Building(size, type, pos);
	BuildingRegion *region = new BuildingRegion(size, pos);

	// Map region to building
	mBuildingMap.emplace(region, building);

	// Add region and building to respective container
	mRegions.push_back(region);
	mBuildings.push_back(building);
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
	for (auto it : mBuildings)
		it->render(window);

	for (auto it : mAgents)
		it->render(window);
}
