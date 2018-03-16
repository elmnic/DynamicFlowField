#include "Toolbox.h"

static sf::Vector2i mMapDimensions;
static std::string mLevelDebug = "resources/Debug.dat";
static std::string mLevel1 = "resources/Debug.dat";
static sf::RenderWindow* mWindow;
static sf::Time mTime;
static sf::Vector2f mBlockSize;

static std::string mFontFile = "resources/Freeroad Regular.ttf";
static sf::Font mFont;

static sf::Texture mTextureBuilding;
static sf::Texture mTextureWall;
static sf::Texture mTextureAgent;
static std::string mTextureFileBuilding = "resources/hus.png";
static std::string mTextureFileWall = "resources/vagg.png";
static std::string mTextureFileAgent = "resources/gubbe.png";

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
		return mLevelDebug;
		break;
	case Toolbox::Level1:
		return mLevel1;
		break;
	case Toolbox::Level2:
		break;
	default:
		return mLevelDebug;
		break;
	}
	return mLevelDebug;
}

Toolbox::Toolbox()
{
	mFont.loadFromFile(mFontFile);
	mTextureBuilding.loadFromFile(mTextureFileBuilding);
	mTextureWall.loadFromFile(mTextureFileWall);
	mTextureAgent.loadFromFile(mTextureFileAgent);
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

sf::Font& Toolbox::getFont()
{
	return mFont;
}

sf::Texture& Toolbox::getTexture(TextureCode code)
{
	switch (code)
	{
	case Toolbox::BUILDING:
		return mTextureBuilding;
		break;
	case Toolbox::WALL:
		return mTextureWall;
		break;
	case Toolbox::AGENT:
		return mTextureAgent;
		break;
	default:
		return mTextureBuilding;
		break;
	}
}

std::string Toolbox::floatToString(float f)
{
	std::ostringstream stringTemp;
	stringTemp << f;
	return stringTemp.str();
}

// Convert mouse position to a coordinate on the 2D grid
sf::Vector2i Toolbox::globalToIndexCoords(sf::Vector2i pos)
{
	float xTileSize = getMapBlockSize().x;
	float yTileSize = getMapBlockSize().y;

	int mouseX = (int)(pos.x / xTileSize);
	int mouseY = (int)(pos.y / yTileSize);

	sf::Vector2i mouseIndex(mouseX, mouseY);
	return mouseIndex;
}
