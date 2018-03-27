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

	// Add itself to poly points
	sf::Vector2f middle = getMiddleOfBuildingGlobal();
	sf::Vector2i middleIndx = Toolbox::globalToIndexCoords(middle);
	addPolyPoint(middleIndx);
}


Building::~Building()
{
	clearPolyPoint();
}

void Building::update()
{
	if (mHealth <= 0)
		kill();
}

void Building::render(sf::RenderWindow& window)
{

	//window.draw(mSprite);

	// Draw lines between polygon points
	if (mRenderIndices)
	{
		for (size_t i = 0; i < mIndices.size() - 1; i++)
		{
			sf::Vector2f start = Toolbox::localToGlobalCoords(mIndices[i].coords);
			             start = Toolbox::getMiddleOfBlock(start);
			sf::Vector2f next  = Toolbox::localToGlobalCoords(mIndices[i + 1].coords);
			             next  = Toolbox::getMiddleOfBlock(next);
			LineRenderer::instance()->renderLine(start, next);
		}
		sf::Vector2f start = Toolbox::localToGlobalCoords(mIndices.back().coords);
		             start = Toolbox::getMiddleOfBlock(start);
		sf::Vector2f next  = Toolbox::localToGlobalCoords(mIndices.front().coords);
		             next  = Toolbox::getMiddleOfBlock(next);
		LineRenderer::instance()->renderLine(start, next);
	}

	if (Toolbox::getRenderClosestPoints())
		for (size_t i = 0; i < mClosestPointTargets.size(); i+=2)
			LineRenderer::instance()->renderLine(mClosestPointTargets[i], mClosestPointTargets[i + 1]);
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
	sf::Vector2f targetMid = Toolbox::getMiddleOfBlock(globalTarget);
	sf::Vector2f vectorReturn(globalPos);
	for (int i = 0; i < mSize; i++)
	{
		for (int j = 0; j < mSize; j++)
		{
			sf::Vector2f bGlobal(start.x + blockSize.x * i, start.y + blockSize.y * j);
			// If other distance is shorter than current shortest, update shortest
			if (Toolbox::getDistance(targetMid, bGlobal) <= Toolbox::getDistance(targetMid, vectorReturn))
				vectorReturn = bGlobal;
		}
	}
	mClosestPointTargets.push_back(targetMid);
	mClosestPointTargets.push_back(vectorReturn);
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
bool Building::addPolyPoint(sf::Vector2i point)
{
	Building::Point P;
	P.coords.x = point.x;
	P.coords.y = point.y;
	mIndices.push_back(P);
	//std::cout << "mIndices size: " << mIndices.size() << "\n";

	// Remove old points that are now inside polygon and create a convex hull surrounding all other points
	convex_hull();

	bool inside = isPointInPoly(point);
	return inside;
}


void Building::convex_hull()
{
	std::vector<Point> P = mIndices;
	size_t n = P.size(), k = 0;
	if (n <= 3) return;
	std::vector<Point> H(2 * n);

	// Sort points lexicographically
	std::sort(P.begin(), P.end());

	// Build lower hull
	for (size_t i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0) 
			k--;
		H[k++] = P[i];
	}

	// Build upper hull
	for (size_t i = n - 1, t = k + 1; i > 0; --i) {
		while (k >= t && cross(H[k - 2], H[k - 1], P[i - 1]) <= 0) 
			k--;
		H[k++] = P[i - 1];
	}

	H.resize(k - 1);
	mIndices = H;
}

// Only keep outermost points
void Building::sortPolyPoints()
{
}

bool Building::isPointInPoly(sf::Vector2i & point)
{
	std::vector<float> vertX;
	std::vector<float> vertY;

	// Add stored points' global coords to test vector
	for (auto point : mIndices)
	{
		sf::Vector2f global = Toolbox::localToGlobalCoords(point.coords);
		sf::Vector2f middle = Toolbox::getMiddleOfBlock(global);
		vertX.push_back(middle.x);
		vertY.push_back(middle.y);
	}
	// Test new point
	sf::Vector2f global = Toolbox::localToGlobalCoords(point);
	sf::Vector2f middle = Toolbox::getMiddleOfBlock(global);
	int inside = Toolbox::pointInPoly(mIndices.size(), vertX, vertY, middle.x, middle.y);
	//std::cout << "point in polygon: " << inside << "\n";
	/*std::cout << "Size of block: " << Toolbox::getMapBlockSize().x << "\nSize of textureRect: " << mSprite.getTextureRect().width << "\n";*/

	return inside;
}

void Building::clearPolyPoint()
{
	Point temp = mIndices[0];
	mIndices.clear();
	mIndices.push_back(temp);
}


void Building::toggleIndices()
{
	//std::cout << "Nr of indices: " << mIndices.size() << "\n";
	mRenderIndices = !mRenderIndices;
	if (mRenderIndices)
		mSprite.setColor(sf::Color(0, 128, 34));
	else
		mSprite.setColor(sf::Color::White);
}

int Building::cross(const Point & O, const Point & A, const Point & B)
{
	return (A.coords.x - O.coords.x) * (B.coords.y - O.coords.y) - (A.coords.y - O.coords.y) * (B.coords.x - O.coords.x);
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
