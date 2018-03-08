#pragma once

#include <SFML\Graphics.hpp>

struct Toolbox
{

	enum StringCode { MapSize, Offensive, Defensive, Agent, Null };
	enum BuildingType { OFFENSIVE, DEFENSIVE };
	enum LevelCode { LevelDebug, Level1, Level2 };

	static Toolbox* instance();

	static std::string getLevel(LevelCode code);

	static void setMapDimensions(sf::Vector2i dim);
	static sf::Vector2i& getMapDimensions();
	static sf::Vector2f& getMapBlockSize();

	static sf::RenderWindow& getWindow();
	static void setWindow(sf::RenderWindow* window);

	static void setDeltaTime(sf::Time time);
	static sf::Time& getDeltaTime();
};
