#pragma once
#include <vector>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include "FlowGenerator.h"
#include "PathPlanner.h"
#include "Entity.h"
#include "EntityManager.h"


// 2D-vector containing the IDs of buildings and ground
typedef std::vector<std::vector<int>> MapMain;

// 2D-vector containing direction vectors
typedef std::vector<std::vector<sf::Vector2f>> MapFlow;


class Map
{
public:

	static Map* instance();
	~Map();

	//TODO: bool return maybe probably not
	void loadMap(std::string &mapName);
	void unloadMap();
	void saveMap(); //TODO:

	void render(sf::RenderWindow& window);


	MapMain& getMap() { return mMapMain; }
	MapFlow& getMapFlow() { return mMapFlow; }
	int getWidth() { return mMapMain[0].size(); }
	int getHeight() { return mMapMain.size(); }

private:
	Map();
	MapMain mMapMain;
	MapFlow mMapFlow;

	EntityManager *mEntityManager;

	// Converts string to enum
	Toolbox::StringCode hashit(std::string const& str);

	void setMapSize(std::string line);
	void loadBuilding(std::string line, Toolbox::BuildingType type);

};

