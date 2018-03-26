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
	enum LevelCode { LevelDebug, Level1, Level2, Level3, Level4, Level5, Level6, Level7, Level8, Level9	};
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
	static std::string boolToString(bool value);

	static sf::Vector2i globalToIndexCoords(const sf::Vector2f& pos);
	static sf::Vector2f localToGlobalCoords(const sf::Vector2i& localPos);

	static sf::Vector2f getMiddleOfBlock(sf::Vector2f& globalPos);

	// Algorithm from of https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
	static int pointInPoly(int nrOfVerts, std::vector<float>& vertX, std::vector<float>& vertY, float testX, float testY);

	static bool getGenerateDynamicFlow();
	static void toggleGenerateDynamicFlow();

	static float getMagnitude(sf::Vector2f& vec);
	static float getDistance(sf::Vector2f& a, sf::Vector2f& b);
	static sf::Vector2f normalize(sf::Vector2f& vec);
	static float getAngleBetween(sf::Vector2f& vecA, sf::Vector2f& vecB);
	static float getDot(sf::Vector2f& vecA, sf::Vector2f& vecB);
	static float getDet(sf::Vector2f& vecA, sf::Vector2f& vecB);
	static float cross(sf::Vector2f& a, sf::Vector2f& b, sf::Vector2f& point);

	// Toggle rendering stuff
	static bool getRenderWeights();
	static void toggleRenderWeights();
	static bool getRenderClosestPoints();
	static void toggleRenderClosestPoints();
	static bool getRenderFlow();
	static void toggleRenderFlow();
	static bool getRenderConfirmed();
	static void toggleRenderConfirmed();
	static bool getRenderTexts();
	static void toggleRenderTexts();

	// Toggle ending the scenario
	static bool isFinished();
	static void setIsFinished(bool value);

	static bool isFrameRateLocked();
	static void toggleFrameRateLocked();
	static void setFrameRateLocked();

};
