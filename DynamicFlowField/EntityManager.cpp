#include "EntityManager.h"
#include "AgentSpawner.h"


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
	exit();
}

void EntityManager::removeDeadEntities()
{
	// Deletes all dead buildings
	EntityVector buildings = mBuildings;
	mBuildings.clear();
	for (auto it : buildings)
	{
		if (it->isAlive())
			mBuildings.push_back(it);
		else
			delete it;
	}

	// Deletes all dead agents
	EntityVector agents = mAgents;
	mAgents.clear();
	for (auto it : agents)
	{
		if (it->isAlive())
			mAgents.push_back(it);
		else
			delete it;
	}
}

void EntityManager::createBuilding(int size, Toolbox::BuildingType type, sf::Vector2i pos)
{
	Building *building = new Building(size, type, pos);

	// Pair all coordinates in the region of the building to the building itself
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			Point p(pos.x + i, pos.y + j);
			mBuildingMap.insert(std::make_pair(p, building));
		}
	}

	// Add building to vector
	mBuildings.push_back(building);
}

void EntityManager::createAgent(sf::Vector2i startPos)
{
	Agent *agent = new Agent(startPos);
	mAgents.push_back(agent);
}

void EntityManager::queueAgent(sf::Vector2i startPos, float spawnTime)
{
	// Actually add to AgentSpawner here
	AgentSpawner::instance()->queueAgent(startPos, spawnTime);
}


void EntityManager::startAgentSpawner()
{
	AgentSpawner::instance()->run();
}

// Search a point and return its building if it exists
Building * EntityManager::isBuilding(Point point)
{
	BuildingMap::iterator it = mBuildingMap.find(point);
	if (it != mBuildingMap.end())
		return it->second;
	else
		return nullptr;
}

void EntityManager::update()
{
	AgentSpawner::instance()->update();

	for (auto it : mBuildings)
		it->update();

	for (auto it : mAgents)
		it->update();

	removeDeadEntities();
}

void EntityManager::render(sf::RenderWindow & window)
{
	for (auto it : mBuildings)
		it->render(window);

	for (auto it : mAgents)
		it->render(window);
}

void EntityManager::exit()
{
	for (auto it : mBuildings)
		delete it;

	for (auto it : mAgents)
		delete it;

	for (std::map<Point, Building*>::iterator it = mBuildingMap.begin(), next_it = mBuildingMap.begin(); it != mBuildingMap.end(); it = next_it)
	{
		next_it = it; ++next_it;
		mBuildingMap.erase(it);
	}

	mBuildings.clear();
	mAgents.clear();
	mBuildingMap.clear();

	AgentSpawner::instance()->clear();
}
