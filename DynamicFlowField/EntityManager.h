#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include <map>

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

	void exit();
	void clearAgents();
	// Removes stored points from buildings and the Confirmed-buildings
	void clearConfirmed();

	void startAgentSpawner();

	// Returns a pointer to the building if it exists, otherwise it returns nullptr
	Building* isBuilding(Point point);

private:
	EntityManager();

	void removeDeadEntities();

	// Entities
	EntityVector mBuildings;
	EntityVector mConfirmed;
	EntityVector mAgents;

	// Building entities mapped to points
	BuildingMap mBuildingMap;

	// Render texture for static buildings
	sf::RenderTexture mBuildingTexture;
	sf::RenderTexture mConfirmedTexture;

};

