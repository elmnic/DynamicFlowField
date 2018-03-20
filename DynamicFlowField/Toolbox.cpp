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
static sf::Texture mTextureConfirmed;
static std::string mTextureFileBuilding = "resources/hus.png";
static std::string mTextureFileWall = "resources/vagg.png";
static std::string mTextureFileAgent = "resources/gubbe.png";
static std::string mTextureFileConfirmed = "resources/confirmed.png";

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
	// Load textures
	mFont.loadFromFile(mFontFile);
	mTextureBuilding.loadFromFile(mTextureFileBuilding);
	mTextureWall.loadFromFile(mTextureFileWall);
	mTextureAgent.loadFromFile(mTextureFileAgent);
	mTextureConfirmed.loadFromFile(mTextureFileConfirmed);
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
	case Toolbox::POLY:
		return mTextureConfirmed;
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
sf::Vector2i Toolbox::globalToIndexCoords(sf::Vector2f& pos)
{
	float xTileSize = getMapBlockSize().x;
	float yTileSize = getMapBlockSize().y;

	int mouseX = (int)(pos.x / xTileSize);
	int mouseY = (int)(pos.y / yTileSize);

	sf::Vector2i mouseIndex(mouseX, mouseY);
	return mouseIndex;
}

sf::Vector2f Toolbox::localToGlobalCoords(sf::Vector2i& localPos)
{
	float globalX = localPos.x * getMapBlockSize().x;
	float globalY = localPos.y * getMapBlockSize().y;

	sf::Vector2f global(globalX, globalY);
	return global;
}

sf::Vector2f Toolbox::getMiddleOfBlock(sf::Vector2f & globalPos)
{
	sf::Vector2f middle(globalPos.x + getMapBlockSize().x / 2, globalPos.y + getMapBlockSize().y / 2);
	return middle;
}

int Toolbox::pointInPoly(int nrOfVerts, std::vector<float>& vertX, std::vector<float>& vertY, float testX, float testY)
{
	int i, j, c = 0;
	for (i = 0, j = nrOfVerts - 1; i < nrOfVerts; j = i++) {
		if (((vertY[i]>testY) != (vertY[j]>testY)) &&
			(testX < (vertX[j] - vertX[i]) * (testY - vertY[i]) / (vertY[j] - vertY[i]) + vertX[i]))
			c = !c;
	}
	return c;
}
