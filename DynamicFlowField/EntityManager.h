#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include <map>

#include "Toolbox.h"
#include "Agent.h"
#include "Building.h"

class EntityManager
{
public:

	typedef std::pair<int, int> Point;
	typedef std::vector<Entity*> EntityVector;
	typedef std::map<Point, Building*> BuildingMap;

	static EntityManager* instance();
	~EntityManager();

	void createBuilding(int size, Toolbox::BuildingType type, sf::Vector2i pos);
	void createAgent(sf::Vector2i startPos);
	void update();
	void render(sf::RenderWindow &window);

	Building* isBuilding(Point point);

private:
	EntityManager();


	// Entities
	EntityVector mBuildings;
	EntityVector mAgents;

	// Map coordinate to specific building
	BuildingMap mBuildingMap;
};

