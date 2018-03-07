#include "Toolbox.h"

static sf::Vector2i mMapDimensions;
static std::string mDebugLevel = "resources/Debug.dat";
static std::string mLevel1 = "resources/Debug.dat";


Toolbox* Toolbox::instance()
{
	Toolbox instance;
	return &instance;
}

std::string Toolbox::getLevel(LevelCode code)
{
	switch (code)
	{
	case Toolbox::LevelDebug:
		return mDebugLevel;
		break;
	case Toolbox::Level1:
		return mLevel1;
		break;
	case Toolbox::Level2:
		break;
	default:
		return mDebugLevel;
		break;
	}
	return mDebugLevel;
}


void Toolbox::setMapDimensions(sf::Vector2i dim)
{
	mMapDimensions = dim;
}

sf::Vector2i& Toolbox::getMapDimensions()
{
	return mMapDimensions;
}