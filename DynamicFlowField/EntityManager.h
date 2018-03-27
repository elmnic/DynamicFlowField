#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include <map>
#include <algorithm>

#include "Toolbox.h"
#include "Agent.h"
#include "Building.h"

class AgentSpawner;

class EntityManager
{
public:

	typedef std::pair<int, int> Point;
	typedef std::vector<Entity*> EntityVector;
	typedef std::map<Point, Building*> BuildingMap;

	static EntityManager* instance();
	~EntityManager();

	void updateBuildingTexture();
	void updateConfirmedTexture();
	void createBuilding(int size, Toolbox::BuildingType type, sf::Vector2i pos);
	void createAgent(sf::Vector2i startPos);
	void createConfirmed(sf::Vector2i pos);
	void queueAgent(sf::Vector2i startPos, float spawnTime);
	void update();
	void render(sf::RenderWindow &window);

	int getNrOfAgents() { return mAgents.size(); }

	// Removes stored points from buildings and the Confirmed-buildings
	void exit();
	void clearAgents();
	void clearConfirmed();
	void clearPolyPoints();
	void clearBuildings();

	void startAgentSpawner();

	// Returns a pointer to the building if it exists, otherwise it returns nullptr
	Building* isBuilding(Point point);

	// For editing map
	void decreaseSize() { mCurrentSize = mCurrentSize > 1 ? mCurrentSize = mCurrentSize - 1 : mCurrentSize = 1; }
	void increaseSize() { mCurrentSize++; }
	void setCurrentBuildingType(Toolbox::BuildingType type) { mCurrentBuildingType = type; }
	void editorAddBuilding(const sf::Vector2i& pos);
	int &getCurrentSize() { return mCurrentSize; }
	void removeBuilding(Building* building);
	EntityVector& getBuildings() { return mBuildings; }

private:
	EntityManager();

	void checkCollision();
	void checkCollisionDir(Agent* agent, Building* building);
	void removeDeadEntities();


	bool mFinishedScenario = false;

	// Entities
	EntityVector mBuildings;
	EntityVector mConfirmed;
	EntityVector mAgents;

	bool mBuildingsKilled = false;

	// Building entities mapped to points
	BuildingMap mBuildingMap;

	// Render texture for static buildings
	sf::RenderTexture mBuildingTexture;
	sf::RenderTexture mConfirmedTexture;

	// For editing map
	int                   mCurrentSize = 1;
	Toolbox::BuildingType mCurrentBuildingType = Toolbox::BuildingType::OFFENSIVE;

};

