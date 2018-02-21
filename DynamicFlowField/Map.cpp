#include "Map.h"



Map::Map()
{
}


Map::~Map()
{
}

void Map::loadMap(std::string & mapName)
{
	std::ifstream mapFile(mapName);
	std::vector<int> vectorTemp;
	std::string str;

	while (getline(mapFile, str))
	{
		std::istringstream ss(str);
		int in;
		while (ss >> in)
		{
			vectorTemp.push_back(in);
		}
		mMapMain.push_back(vectorTemp);
		vectorTemp.clear();
	}
}
