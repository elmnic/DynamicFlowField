#include "Building.h"

#include <iostream>

Building::Building(int size, Toolbox::BuildingType type, sf::Vector2i pos) :
	mSize(size),
	mType(type),
	mPosition(pos),
	mRenderIndices(false)
{
	// Set texture and scale it according to building size
	mSprite.setTexture(Toolbox::instance()->getTexture(buildingToTexture(type)));
	float sizeX = Toolbox::getMapBlockSize().x;
	float sizeY = Toolbox::getMapBlockSize().y;

	float scaleX = mSprite.getTexture()->getSize().x / sizeX;
	float scaleY = mSprite.getTexture()->getSize().y / sizeY;

	// Reset sprite size to fit block
	mSprite.scale(sf::Vector2f(1 / scaleX, 1 / scaleY));

	// Scale sprite to accomodate building size 
	mSprite.scale(sf::Vector2f((float)size, (float)size));
	mSprite.setPosition(sf::Vector2f((float)mPosition.x * sizeX, (float)mPosition.y * sizeY));
}


Building::~Building()
{
}

void Building::update()
{
}

void Building::render(sf::RenderWindow& window)
{
	if (mRenderIndices)
		mSprite.setColor(sf::Color(0, 128, 34));
	else
		mSprite.setColor(sf::Color::White);

	window.draw(mSprite);

	// Draw lines between building middle and indices middle
	if (mRenderIndices)
	{
		sf::Vector2f startPoint = getMiddleOfBuildingGlobal();
		for (auto point : mIndices)
		{
			sf::Vector2f globalCoords = Toolbox::localToGlobalCoords(point);
			LineRenderer::instance()->renderLine(startPoint, Toolbox::getMiddleOfBlock(globalCoords));
		}
	}
}

void Building::kill()
{
	mAlive = false;
}

sf::Vector2i Building::getPositionClosest(sf::Vector2f& globalTarget)
{
	// Iterate through all building blocks and check which one is the closest
	sf::Vector2f globalPos = Toolbox::localToGlobalCoords(mPosition);
	sf::Vector2f start     = Toolbox::getMiddleOfBlock(globalPos);
	sf::Vector2f blockSize = Toolbox::getMapBlockSize();
	sf::Vector2f vectorReturn(mPosition);
	for (int i = 0; i < mSize; i++)
	{
		for (int j = 0; j < mSize; j++)
		{
			sf::Vector2f bGlobal(start.x + blockSize.x * i, start.y + blockSize.y * j);
			// If other distance is shorter than current shortest, update shortest
			if (Toolbox::getDistance(globalTarget, bGlobal) < Toolbox::getDistance(globalTarget, vectorReturn))
				vectorReturn = bGlobal;
		}
	}
	return Toolbox::globalToIndexCoords(vectorReturn);
}

sf::Vector2f Building::getMiddleOfBuildingGlobal()
{
	float xBlockSize = Toolbox::getMapBlockSize().x;
	float yBlockSize = Toolbox::getMapBlockSize().y;
	float xGlobalMid = mPosition.x * xBlockSize + (xBlockSize * mSize) / 2;
	float yGlobalMid = mPosition.y * yBlockSize + (yBlockSize * mSize) / 2;
	sf::Vector2f middleCoords(xGlobalMid, yGlobalMid);
	return middleCoords;
}

// Store polygon indices in global coords
void Building::addPolyPoint(sf::Vector2i point)
{
	// Building has less than two points, no covering polygon possible
	if (mIndices.size() < 2)
		mIndices.push_back(point);
	else
	{
		std::vector<float> vertX;
		std::vector<float> vertY;
		// Add building middle
		vertX.push_back(this->getMiddleOfBuildingGlobal().x);
		vertY.push_back(this->getMiddleOfBuildingGlobal().y);
		
		// Add global point coords to test vector
		for (auto point : mIndices)
		{
			sf::Vector2f global = Toolbox::localToGlobalCoords(point);
			sf::Vector2f middle = Toolbox::getMiddleOfBlock(global);
			vertX.push_back(middle.x);
			vertY.push_back(middle.y);
		}
		// Test new point
		sf::Vector2f global = Toolbox::localToGlobalCoords(point);
		sf::Vector2f middle = Toolbox::getMiddleOfBlock(global);
		int inOut = Toolbox::pointInPoly(mIndices.size() + 1, vertX, vertY, middle.x, middle.y);
		std::cout << "point in polygon: " << inOut << "\n";
		if (!inOut)
			mIndices.push_back(point);

		// Remove old points that are now inside polygon
		sortPolyPoints();
		
	}
}

void Building::sortPolyPoints()
{
	std::vector<float> vertX;
	std::vector<float> vertY;

	std::vector<sf::Vector2i> indices = mIndices;
	mIndices.clear();

	for (size_t i = 0; i < indices.size(); i++)
	{
		// Add building middle
		vertX.push_back(this->getMiddleOfBuildingGlobal().x);
		vertY.push_back(this->getMiddleOfBuildingGlobal().y);

		// Add all other points to poly test vector
		for (size_t j = 0; j < indices.size(); j++)
		{
			// Skip itself
			if (i != j)
			{
				sf::Vector2f global = Toolbox::localToGlobalCoords(indices[j]);
				sf::Vector2f middle = Toolbox::getMiddleOfBlock(global);
				vertX.push_back(middle.x);
				vertY.push_back(middle.y);
			}
		}

		// Create test point
		sf::Vector2f testPoint = Toolbox::localToGlobalCoords(indices[i]);
		sf::Vector2f testPointMiddle = Toolbox::getMiddleOfBlock(testPoint);

		// Test if old point is inside polygon. Keep those that are still outside
		int inOut = Toolbox::pointInPoly(indices.size(), vertX, vertY, testPointMiddle.x, testPointMiddle.y);
		if (!inOut)
			mIndices.push_back(indices[i]);

		vertX.clear();
		vertY.clear();
	}
}

void Building::clearPolyPoint()
{
	mIndices.clear();
}

void Building::toggleIndices()
{
	std::cout << "Nr of indices: " << mIndices.size() << "\n";
	mRenderIndices = !mRenderIndices;
}

Toolbox::TextureCode Building::buildingToTexture(Toolbox::BuildingType type)
{
	switch (type)
	{
	case Toolbox::OFFENSIVE:
		return Toolbox::TextureCode::BUILDING;
		break;
	case Toolbox::DEFENSIVE:
		return Toolbox::TextureCode::WALL;
		break;
	case Toolbox::POLYPOINT:
		return Toolbox::TextureCode::POLY;
		break;
	default:
		break;
	}
	return Toolbox::TextureCode();
}
