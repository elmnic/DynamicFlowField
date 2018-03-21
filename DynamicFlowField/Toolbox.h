#pragma once

#include <SFML\Graphics.hpp>
#include <atomic>
#include <sstream>
#include <cmath>
#include <iostream>

struct Toolbox
{
private:
	Toolbox();

public:
	enum StringCode { MapSize, Offensive, Defensive, Agent, Null };
	enum BuildingType { OFFENSIVE, DEFENSIVE, POLYPOINT };
	enum LevelCode { LevelDebug, Level1, Level2 };
	enum TextureCode { BUILDING, WALL, AGENT, POLY };

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

	static sf::Vector2i globalToIndexCoords(sf::Vector2f& pos);
	static sf::Vector2f localToGlobalCoords(sf::Vector2i& localPos);

	// Returns middle of block using global coords
	static sf::Vector2f getMiddleOfBlock(sf::Vector2f& globalPos);

	static int pointInPoly(int nrOfVerts, std::vector<float>& vertX, std::vector<float>& vertY, float testX, float testY);

	static bool getDynamicOrStatic();
	// Will toggle between dynamic and static flow field
	static void SetDynamicOrStatic();

	static float getMagnitude(sf::Vector2f& vec);
	static float getDistance(sf::Vector2f& a, sf::Vector2f& b);

};
