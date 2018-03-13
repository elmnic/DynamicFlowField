#include "Toolbox.h"
#include <atomic>

static sf::Vector2i mMapDimensions;
static std::string mDebugLevel = "resources/Debug.dat";
static std::string mLevel1 = "resources/Debug.dat";
static sf::RenderWindow* mWindow;
static sf::Time mTime;
static sf::Vector2f mBlockSize;

static std::atomic<bool> mTerminateSimulation = false;

Toolbox* Toolbox::instance()
{
	static Toolbox instance;
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

sf::Vector2f& Toolbox::getMapBlockSize()
{
	mBlockSize = sf::Vector2f((float)(mWindow->getSize().x / mMapDimensions.x), (float)(mWindow->getSize().y / mMapDimensions.y));
	return mBlockSize;
}

sf::RenderWindow& Toolbox::getWindow()
{
	return *mWindow;
}

void Toolbox::setWindow(sf::RenderWindow* window)
{
	mWindow = window;
}

void Toolbox::setDeltaTime(sf::Time time)
{
	mTime = time;
}

sf::Time& Toolbox::getDeltaTime()
{
	return mTime;
}

bool Toolbox::getTerminateSimulation()
{
	return mTerminateSimulation;
}

void Toolbox::setTerminateSimulation(bool value)
{
	mTerminateSimulation = value;
}