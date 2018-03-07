#pragma once

#include <SFML\Graphics.hpp>

struct Toolbox
{

	enum StringCode { MapSize, Offensive, Defensive, Null };
	enum BuildingType { OFFENSIVE, DEFENSIVE };
	enum LevelCode { LevelDebug, Level1, Level2 };

	static Toolbox* instance();

	static std::string getLevel(LevelCode code);

	static void setMapDimensions(sf::Vector2i dim);
	static sf::Vector2i& getMapDimensions();


};
