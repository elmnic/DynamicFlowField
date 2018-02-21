#pragma once
#include <vector>
#include <SFML\System\Vector2.hpp>
#include "FlowGenerator.h"
#include "PathPlanner.h"

// 2D-vector containing the IDs of buildings and ground
typedef std::vector<std::vector<int>> MapMain;

// 2D-vector containing direction vectors
typedef std::vector<std::vector<sf::Vector2f>> MapFlow;

class Map
{
public:
	Map();
	~Map();

	void loadMap(std::string &mapName);

private:
	MapMain mMapMain;
	MapFlow mMapFlow;
};

