#pragma once

#include <SFML\Graphics.hpp>
#include <atomic>
#include <sstream>

struct Toolbox
{
private:
	Toolbox();

public:
	enum StringCode { MapSize, Offensive, Defensive, Agent, Null };
	enum BuildingType { OFFENSIVE, DEFENSIVE };
	enum LevelCode { LevelDebug, Level1, Level2 };
	enum TextureCode { BUILDING, WALL, AGENT };

	static Toolbox* instance();

	static std::string getLevel(LevelCode code);

	static void setMapDimensions(sf::Vector2i dim);
	static sf::Vector2i& getMapDimensions();
	static sf::Vector2f& getMapBlockSize();

	static sf::RenderWindow& getWindow();
	static void setWindow(sf::RenderWindow* window);

	static void setDeltaTime(sf::Time time);
	static sf::Time& getDeltaTime();

	static bool getTerminateSimulation();
	static void setTerminateSimulation(bool value);

	static sf::Font& getFont();

	static sf::Texture& getTexture(TextureCode code);

	static std::string floatToString(float f);

	static sf::Vector2i globalToIndexCoords(sf::Vector2i pos);

};
