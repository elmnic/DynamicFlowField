#pragma once
#include <vector>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>
#include <fstream>
#include <sstream>

#include "FlowGenerator.h"
#include "PathPlanner.h"



// 2D-vector containing the IDs of buildings and ground
typedef std::vector<std::vector<int>> MapMain;

// 2D-vector containing direction vectors
typedef std::vector<std::vector<sf::Vector2f>> MapFlow;

class Map
{
public:
	enum BuildingType { OFFENSIVE, DEFENSIVE };

	static Map* instance();
	~Map();

	//TODO: bool return
	void loadMap(std::string &mapName); // TODO: Change map file to contain building coordinates and type inste
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


};

