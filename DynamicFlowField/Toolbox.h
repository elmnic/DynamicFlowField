#pragma once

#include <SFML\Graphics.hpp>

struct Toolbox
{

	enum StringCode { MapSize, Offensive, Defensive, Null };
	enum BuildingType { OFFENSIVE, DEFENSIVE };

	static Toolbox* instance();

	static void setMapDimensions(sf::Vector2i dim);
	static sf::Vector2i& getMapDimensions();


};
