#pragma once
#include <vector>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include "Entity.h"
#include "EntityManager.h"
#include "PathPlanner.h"
#include "FlowGenerator.h"


// 2D-vector containing the IDs of buildings and ground
typedef std::vector<std::vector<int>> MapMain;


class Map
{
public:

	static Map* instance();
	~Map();

	void loadMap(Toolbox::LevelCode scenario);
	void unloadMap();
	void saveMap(); //TODO:

	void render(sf::RenderWindow& window);

	void startSimulation();

	MapMain& getMap() { return mMapMain; }
	int getWidth() { return mMapMain[0].size(); }
	int getHeight() { return mMapMain.size(); }

private:
	Map();
	MapMain mMapMain;
	EntityManager *mEntityManager;

	sf::RenderTexture mRenderTexture;

	// Converts string to enum
	Toolbox::StringCode hashit(std::string const& str);

	void setMapSize(std::string line);
	void loadBuilding(std::string line, Toolbox::BuildingType type);
	void loadAgent(std::string line);

};

