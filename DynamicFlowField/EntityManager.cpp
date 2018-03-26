#include "EntityManager.h"
#include "AgentSpawner.h"


EntityManager* EntityManager::instance()
{
	static EntityManager instance;
	return &instance;
}


EntityManager::EntityManager()
{
	mBuildingTexture.create(Toolbox::getWindow().getSize().x, Toolbox::getWindow().getSize().y);
	mConfirmedTexture.create(Toolbox::getWindow().getSize().x, Toolbox::getWindow().getSize().y);
}

EntityManager::~EntityManager()
{
	exit();
}

void EntityManager::checkCollision()
{
	// Loop through all agents and check if they overlap with any buildings
	for (auto building : mBuildings)
	{
		Building *b = (Building*)building;
		if (b->getType() == Toolbox::BuildingType::OFFENSIVE)
		{
			for (auto agent : mAgents)
			{
				if (agent->getSprite().getGlobalBounds().intersects(building->getSprite().getGlobalBounds()))
				{
					agent->kill();
					b->damage();
				}
			}
		}
	}
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
		{
			// Building is to be removed.
			delete it;
			Toolbox::setIsFinished(true);
			updateBuildingTexture();
		}
	}

	if (mBuildings.empty())
		mFinishedScenario = true;

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
void EntityManager::updateBuildingTexture()
{
	mBuildingTexture.clear(sf::Color(0, 0, 0, 0));

	for (auto building : mBuildings)
	{
		Building* b = (Building*)building;
		mBuildingTexture.draw(b->getSprite());
	}

	mBuildingTexture.display();
}

void EntityManager::updateConfirmedTexture()
{
	mConfirmedTexture.clear(sf::Color(0, 0, 0, 0));

	for (auto confirmed : mConfirmed)
	{
		Building* b = (Building*)confirmed;
		mConfirmedTexture.draw(b->getSprite());
	}

	mConfirmedTexture.display();
}

//TODO: Update this render texture every time the building entities are removed
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

	// Update the static building texture
	updateBuildingTexture();
}

void EntityManager::createAgent(sf::Vector2i startPos)
{
	Agent *agent = new Agent(startPos);
	mAgents.push_back(agent);
}

void EntityManager::createConfirmed(sf::Vector2i pos)
{
	// Used to check if point has been confirmed
	Building *building = new Building(1, Toolbox::BuildingType::POLYPOINT, pos);
	Point p(pos.x, pos.y);
	mBuildingMap.insert(std::make_pair(p, building));
	mConfirmed.push_back(building);
	updateConfirmedTexture();
}

void EntityManager::queueAgent(sf::Vector2i startPos, float spawnTime)
{
	// Actually add to AgentSpawner here
	AgentSpawner::instance()->queueAgent(startPos, spawnTime);
}


void EntityManager::startAgentSpawner()
{
	AgentSpawner::instance()->run();
	Toolbox::setIsFinished(false);
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
	{
		it->update();
		Agent* agent = (Agent*)it;
		// Target was found dead, search for new 
		if (mBuildingsKilled)
		{
			agent->updatePath();
			mBuildingsKilled = false;
		}

	}

	checkCollision();

	removeDeadEntities();
}

void EntityManager::render(sf::RenderWindow & window)
{
	// Render lines from building
	for (auto it : mBuildings)
	{
		Building* building = (Building*)it;
		if (building->getType() == Toolbox::BuildingType::OFFENSIVE)
			it->render(window);
	}

	// Render building texture
	sf::Sprite staticBuildingTex(mBuildingTexture.getTexture());
	Toolbox::getWindow().draw(staticBuildingTex);
	
	// Render confirmed texture
	if (Toolbox::getRenderConfirmed())
	{
		sf::Sprite staticConfirmedTex(mConfirmedTexture.getTexture());
		Toolbox::getWindow().draw(staticConfirmedTex);
	}

	for (auto it : mAgents)
		it->render(window);
}

void EntityManager::exit()
{
	for (auto it : mBuildings)
		delete it;

	for (BuildingMap::iterator it = mBuildingMap.begin(), next_it = mBuildingMap.begin(); it != mBuildingMap.end(); it = next_it)
	{
		next_it = it; ++next_it;
		mBuildingMap.erase(it);
	}
	mBuildings.clear();
	mBuildingMap.clear();

	clearConfirmed();
	clearAgents();
}

void EntityManager::clearAgents()
{
	for (auto it : mAgents)
		delete it;
	mAgents.clear();
	AgentSpawner::instance()->clear();
}

void EntityManager::clearConfirmed()
{
	for (size_t i = 0; i < mBuildings.size(); i++)
	{
		Building* b = (Building*)mBuildings[i];
		b->clearPolyPoint();
	}

	for (size_t i = 0; i < mConfirmed.size(); i++)
	{
		Building* b = (Building*)mConfirmed[i];
		Point p((int)b->getPosition().x, (int)b->getPosition().y);
		mBuildingMap.erase(p);

		delete mConfirmed[i];
	}
	mConfirmed.clear();
}
