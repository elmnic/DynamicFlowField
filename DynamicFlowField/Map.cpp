#include "Map.h"


Map* Map::instance()
{
	static Map instance;
	return &instance;
}

Map::Map()
{
	mRenderTexture.create(Toolbox::getWindow().getSize().x, Toolbox::getWindow().getSize().y);
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
	float spawnTime = 0;

	// Retrieve spawn time
	std::getline(ss, output, ',');
	spawnTime = std::stof(output);

	// Retrieve coords
	while (std::getline(ss, output, ','))
	{
		temp.push_back(std::stoi(output));
	}
	sf::Vector2i coords(temp[0], temp[1]);

	// Queue agent entity
	mEntityManager->queueAgent(coords, spawnTime);
}


void Map::loadMap(Toolbox::LevelCode scenario)
{
	// Clear map before loading new
	unloadMap();
	mCurrentScenario = scenario;
	std::ifstream mapFile(Toolbox::getLevel(scenario));
	std::vector<int> vectorTemp;
	std::string str;
	char delimiter = ';';

	if (mapFile.fail())
	{
		std::cout << "File doesn't exist!" << "\n";
		return;
	}

	while (std::getline(mapFile, str, delimiter))
	{
		std::istringstream ss(str);
		std::string id;
		
		// Retrieve data portion of line
		std::size_t pos = str.find(",");
		std::string restOfLine = str.substr(pos + 1);
		
		// Retrieve id from line
		std::getline(ss, id, ',');

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

	mapFile.close();

	// Render background tiles to a separate texture 
	float sizeX = Toolbox::getMapBlockSize().x;
	float sizeY = Toolbox::getMapBlockSize().y;
	mRenderTexture.clear();

	for (size_t i = 0; i < mMapMain.size(); i++)
	{
		for (size_t j = 0; j < mMapMain[i].size(); j++)
		{
			sf::Vector2f pos = sf::Vector2f(j * sizeX, i * sizeY);

			sf::RectangleShape rect(sf::Vector2f(sizeX, sizeY));
			rect.setPosition(pos);
			rect.setOutlineColor(sf::Color::Black);
			rect.setOutlineThickness(1.f);
			rect.setFillColor(sf::Color::Green);
			mRenderTexture.draw(rect);
		}
	}

	// Finish drawing to texture
	mRenderTexture.display();

}

void Map::unloadMap()
{
	mMapMain.clear();
	mRenderTexture.clear(sf::Color(0, 0, 0, 0));
	mEntityManager->exit();
	PathPlanner::instance()->clear();
	FlowGenerator::instance()->clear();
}

void Map::saveMap()
{
	std::cout << "Saving map...\n";
	std::ofstream mapFile(Toolbox::getLevel(mCurrentScenario), std::ofstream::trunc);
	EntityManager::EntityVector buildings = EntityManager::instance()->getBuildings();

	// Begin with writing the map size
	mapFile << "Map," + 
		std::to_string(Toolbox::getMapDimensions().x) + 
		"," + 
		std::to_string(Toolbox::getMapDimensions().y) + 
		";";

	for (auto it : buildings)
	{
		Building* b = (Building*)it;
		sf::Vector2i index = Toolbox::globalToIndexCoords(b->getPosition());
		if (b->getType() == Toolbox::BuildingType::OFFENSIVE)
			mapFile << "Off,";
		else if (b->getType() == Toolbox::BuildingType::DEFENSIVE)
			mapFile << "Def,";
		mapFile << std::to_string(b->getSize()) + ",";
		mapFile << std::to_string(index.x) + "," + std::to_string(index.y) + ";";
	}


	mapFile.close();
	std::cout << "Saved\n";
}

void Map::render(sf::RenderWindow& window)
{
	sf::Sprite map(mRenderTexture.getTexture());
	window.draw(map);
}

void Map::startSimulation()
{
	mEntityManager->startAgentSpawner();
}
