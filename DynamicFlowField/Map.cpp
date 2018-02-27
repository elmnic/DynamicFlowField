#include "Map.h"


Map* Map::instance()
{
	static Map instance;
	return &instance;
}

Map::Map()
{
}


Map::~Map()
{
}

void Map::loadMap(std::string & mapName)
{
	// Clear map before loading new
	unloadMap();

	std::ifstream mapFile("resources/Debug.dat");
	std::vector<int> vectorTemp;
	std::string str;
	char delimiter = ';';

	while (getline(mapFile, str, delimiter))
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

void Map::unloadMap()
{
	mMapMain.clear();
}

void Map::render(sf::RenderWindow& window)
{
	float sizeX = 0;
	float sizeY = 0;
	sizeX = window.getSize().x / getWidth();
	sizeY = window.getSize().y / getHeight();

	for (int i = 0; i < mMapMain.size(); i++)
	{
		for (int j = 0; j < mMapMain[i].size(); j++)
		{
			sf::Vector2f pos = sf::Vector2f(j * sizeX, i * sizeY);

			sf::RectangleShape rect(sf::Vector2f(sizeX, sizeY));
			rect.setPosition(pos);
			rect.setOutlineColor(sf::Color::Black);
			rect.setOutlineThickness(2.5f);
			
			switch (mMapMain[i][j]) 
			{
			case 0:
				rect.setFillColor(sf::Color::Green);
				window.draw(rect);
				break;
			case 1:
				rect.setFillColor(sf::Color::Blue);
				window.draw(rect);
				break;
			case 2:
				rect.setFillColor(sf::Color::Magenta);
				window.draw(rect);
				break;
			default:
				break;
			}
		}
	}
}