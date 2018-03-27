#include "EntityManager.h"
#include "AgentSpawner.h"


EntityManager* EntityManager::instance()
{
	static EntityManager instance;
	return &instance;
}


void EntityManager::editorAddBuilding(const sf::Vector2i& pos)
{
	// Create a building if the point is not occupied
	Point p(pos.x, pos.y);
	if (isBuilding(p) == nullptr)
		createBuilding(mCurrentSize, mCurrentBuildingType, pos);
}

void EntityManager::removeBuilding(Building * building)
{
	// Erase the points from the buildingMap
	sf::Vector2i pos = Toolbox::globalToIndexCoords(building->getPosition());
	int size = building->getSize();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			Point p(pos.x + i, pos.y + j);
			mBuildingMap.erase(p);
		}
	}
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

		for (auto agent : mAgents)
		{
			if (agent->getSprite().getGlobalBounds().intersects(building->getSprite().getGlobalBounds()))
			{
				if (b->getType() == Toolbox::BuildingType::OFFENSIVE)
				{
					agent->kill();
					b->damage();
				}
				/*else if (b->getType() == Toolbox::BuildingType::DEFENSIVE)
				{
					checkCollisionDir((Agent*)agent, b);
				}*/
			}
		}
	}
}

void EntityManager::checkCollisionDir(Agent* agent, Building* building)
{
	float player_bottom = agent->getPosition().y + agent->getSprite().getGlobalBounds().height;
	float tiles_bottom = building->getPosition().y + building->getSprite().getGlobalBounds().height;
	float player_right = agent->getPosition().x + agent->getSprite().getGlobalBounds().width;
	float tiles_right = building->getPosition().x + building->getSprite().getGlobalBounds().width;

	float b_collision = tiles_bottom - agent->getPosition().y;
	float t_collision = player_bottom - building->getPosition().y;
	float l_collision = player_right - building->getPosition().x;
	float r_collision = tiles_right - agent->getPosition().x;

	sf::Vector2f offset(0, 0);
	if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision)
	{
		//Top collision
		offset.y = building->getPosition().y - player_bottom;
		//std::cout << "Top\n";
	}
	if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision)
	{
		//Bottom collision
		offset.y = building->getPosition().y + building->getSprite().getGlobalBounds().height - agent->getPosition().y;
		//std::cout << "Bottom\n";
	}
	if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision)
	{
		//Left collision
		offset.x = player_right - building->getPosition().x;
		//std::cout << "Left\n";
	}
	if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision)
	{
		//Right collision
		offset.x = building->getPosition().x + building->getSprite().getGlobalBounds().width - agent->getPosition().x;
		//std::cout << "Right\n";
	}
	agent->moveAgent(offset);
}

void EntityManager::removeDeadEntities()
{
	Toolbox::setBuildingKilled(false);

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
			Building* b = (Building*)it;
			removeBuilding(b);
			b->clearPolyPoint();
			delete it;
			Toolbox::setBuildingKilled(true);
		}
	}
	// Remove Confirmed buildings if an offensive building was destroyed
	if (Toolbox::isBuildingKilled())
	{
		EntityVector buildings = mBuildings;
		mBuildings.clear();
		for (auto it : buildings)
		{
			Building *b = (Building*)it;
			if (b->getType() != Toolbox::BuildingType::POLYPOINT)
				mBuildings.push_back(it);
			else
				delete it;
			b->clearPolyPoint();
		}
	}

	if (Toolbox::isBuildingKilled())
		mConfirmed.clear();

	if (buildings.size() != mBuildings.size())
		updateBuildingTexture();

	// Checks if there are any offensive buildings that could be targeted left
	bool targetsLeft = false;
	for (auto targetBuildingCheck : mBuildings)
	{
		Building *b = (Building*)targetBuildingCheck;
		if (b->getType() == Toolbox::BuildingType::OFFENSIVE)
			targetsLeft = true;
	}

	if (!targetsLeft)
		Toolbox::setIsFinished(true);

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
			mBuildingMap[p] = building;
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
	Toolbox::setIsFinished(false);
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

	Toolbox::setBuildingKilled(false);

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
	clearBuildings();
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
	for (size_t i = 0; i < mConfirmed.size(); i++)
	{
		Building* b = (Building*)mConfirmed[i];
		Point p((int)b->getPosition().x, (int)b->getPosition().y);
		mBuildingMap.erase(p);

		delete mConfirmed[i];
	}
	mConfirmed.clear();
	updateConfirmedTexture();
}

void EntityManager::clearPolyPoints()
{
	for (size_t i = 0; i < mBuildings.size(); i++)
	{
		Building* b = (Building*)mBuildings[i];
		b->clearPolyPoint();
	}
}

void EntityManager::clearBuildings()
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
	updateBuildingTexture();
}
