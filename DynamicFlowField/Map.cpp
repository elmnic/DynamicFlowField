#include "Map.h"


Map* Map::instance()
{
	static Map instance;
	return &instance;
}

Map::Map()
{
	mEntityManager = EntityManager::instance();
}

Map::~Map()
{
}

Toolbox::StringCode Map::hashit(std::string const & str)
{
	if (str == "Map") return Toolbox::StringCode::MapSize;
	if (str == "Off") return Toolbox::StringCode::Offensive;
	if (str == "Def") return Toolbox::StringCode::Defensive;
	if (str == "Age") return Toolbox::StringCode::Agent;

	return Toolbox::StringCode::Null;
}

void Map::setMapSize(std::string line)
{
	// Read coords from the rest of the line and resize map vector
	std::istringstream ss(line);
	std::string output;
	std::vector<int> temp;
	while (std::getline(ss, output, ','))
	{
		temp.push_back(std::stoi(output));
	}
	sf::Vector2i size(temp[0], temp[1]);
	Toolbox::setMapDimensions(size); // Save dimensions in toolbox
	mMapMain.resize(size.y, std::vector<int>(size.x, 0)); // Resize and init map matrix to 0

}

void Map::loadBuilding(std::string line, Toolbox::BuildingType type)
{
	std::istringstream ss(line);
	std::string output;
	std::vector<int> temp;
	int size;

	// Retrieve size
	std::getline(ss, output, ',');
	size = std::stoi(output);

	// Retrieve coords
	while (std::getline(ss, output, ','))
	{
		temp.push_back(std::stoi(output));
	}
	sf::Vector2i coords(temp[0], temp[1]);

	//Create building entity
	mEntityManager->createBuilding(size, type, coords);
}

void Map::loadAgent(std::string line)
{
	std::istringstream ss(line);
	std::string output;
	std::vector<int> temp;

	// Retrieve coords
	while (std::getline(ss, output, ','))
	{
		temp.push_back(std::stoi(output));
	}
	sf::Vector2i coords(temp[0], temp[1]);

	// Create agent entity
	mEntityManager->createAgent(coords);
}


void Map::loadMap(std::string & mapName)
{
	// Clear map before loading new
	unloadMap();

	std::ifstream mapFile(Toolbox::getLevel(Toolbox::LevelCode::LevelDebug));
	std::vector<int> vectorTemp;
	std::string str;
	char delimiter = ';';

	while (std::getline(mapFile, str, delimiter))
	{
		std::cout << str << "\n";
		std::istringstream ss(str);
		std::string id;
		
		// Retrieve data portion of line
		std::size_t pos = str.find(",");
		std::string restOfLine = str.substr(pos + 1);
		
		// Retrieve id from line
		std::getline(ss, id, ',');
		std::cout << "Id: " << id << "\n";

		// Send rest of line to other function for further work
		switch (hashit(id))
		{
		case Toolbox::StringCode::MapSize:
			setMapSize(restOfLine);
			break;
		case Toolbox::StringCode::Offensive:
			loadBuilding(restOfLine, Toolbox::BuildingType::OFFENSIVE);
			break;
		case Toolbox::StringCode::Defensive:
			loadBuilding(restOfLine, Toolbox::BuildingType::DEFENSIVE);
			break;
		case Toolbox::StringCode::Agent:
			loadAgent(restOfLine);
			break;
		default:
			break;
		}
	}
}

void Map::unloadMap()
{
	mMapMain.clear();
}

void Map::render(sf::RenderWindow& window)
{
	float sizeX = (float)(window.getSize().x / Toolbox::getMapDimensions().x);
	float sizeY = (float)(window.getSize().y / Toolbox::getMapDimensions().y);

	for (size_t i = 0; i < mMapMain.size(); i++)
	{
		for (size_t j = 0; j < mMapMain[i].size(); j++)
		{
			sf::Vector2f pos = sf::Vector2f(j * sizeX, i * sizeY);

			sf::RectangleShape rect(sf::Vector2f(sizeX, sizeY));
			rect.setPosition(pos);
			rect.setOutlineColor(sf::Color::Black);
			rect.setOutlineThickness(1.f);
			
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