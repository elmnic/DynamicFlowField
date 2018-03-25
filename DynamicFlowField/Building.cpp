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
}

void Building::update()
{
}

void Building::render(sf::RenderWindow& window)
{

	//window.draw(mSprite);

	// Draw lines between polygon points
	if (mRenderIndices)
	{
		for (int i = 0; i < mIndices.size() - 1; i++)
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
		for (int i = 0; i < mClosestPointTargets.size(); i+=2)
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
	std::cout << "mIndices size: " << mIndices.size() << "\n";

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
	

	////
	//std::vector<sf::Vector2i> mVertices = mIndices;
	//mIndices.clear();
	//sf::Vector2i leftMost = mVertices[0];

	//// Find the leftmost and uppermost vertex
	//for (int i = 0; i < mVertices.size(); i++)
	//{
	//	if (mVertices[i].x < leftMost.x)
	//		leftMost = mVertices[i];
	//	else if (mVertices[i].x == leftMost.x)
	//	{
	//		if (mVertices[i].y < leftMost.y)
	//			leftMost = mVertices[i];
	//	}
	//}
	//// Add the first one back to the vector
	////mIndices.push_back(leftMost);
	//sf::Vector2i current = leftMost;

	//for (size_t i = 0; i < mVertices.size(); i++)
	//{
	//	if (!mIndices.empty())
	//	{
	//	}
	//	for (size_t j = 0; j < mVertices.size(); j++)
	//	{
	//		// Skip itself
	//		if (i == j)
	//			continue;

	//		// Used to check how many points there were on each side
	//		int negative = 0;
	//		int positive = 0;

	//		// Skip points that are already confirmed as outer point
	//		bool alreadyStored = false;
	//		for (auto it : mIndices)
	//		{
	//			if (it == mVertices[j])
	//				alreadyStored = true;
	//		}
	//		if (alreadyStored)
	//			break;

	//		// Check each point
	//		sf::Vector2f vecA = sf::Vector2f(mVertices[i].x, mVertices[i].y);
	//		sf::Vector2f vecB = sf::Vector2f(mVertices[j].x, mVertices[j].y);
	//		for (size_t pointCheck = 0; pointCheck < mVertices.size(); pointCheck++)
	//		{
	//			// Skip themselves
	//			if (j == pointCheck || i == pointCheck)
	//				continue;

	//			// Calculates which side the point is on the line between vecA and vecB
	//			sf::Vector2f point = sf::Vector2f(mVertices[j].x, mVertices[j].y);
	//			float sign = Toolbox::cross(vecA, vecB, point);
	//			if (sign < 0)
	//				negative++;
	//			else if (sign > 0)
	//				positive++;
	//		}
	//		if (negative == 0 || positive == 0)
	//		{
	//			mIndices.push_back(mVertices[j]);
	//		}
	//	}
	//}

	// #########################
	//int iterator = 0;
	//bool backAtStart = false;
	//while (!backAtStart)
	//{
	//	std::cout << "Iterator: " << iterator << "\n";
	//	if (iterator != 0 && mIndices[iterator] == mIndices.front())
	//	{
	//		backAtStart = true;
	//		break;
	//	}

	//	int negative = 0;
	//	int positive = 0;
	//	bool alreadyChecked = false;
	//	for (int i = 0; i < mVertices.size(); i++)
	//	{
	//		for (int j = 0; j < mVertices.size(); j++)
	//		{
	//			if (i == j)
	//				continue;

	//			// Check which side of the line the point is
	//			sf::Vector2f vecA = sf::Vector2f(mIndices[iterator].x, mIndices[iterator].y);
	//			sf::Vector2f vecB = sf::Vector2f(mVertices[i].x, mVertices[i].y);
	//			sf::Vector2f point = sf::Vector2f(mVertices[j].x, mVertices[j].y);
	//			float sign = Toolbox::cross(vecA, vecB, point);
	//			if (sign < 0)
	//				negative++;
	//			else if (sign > 0)
	//				positive++;
	//			//else
	//				// On the line
	//		}
	//		if ((negative == 0 || positive == 0) && !alreadyChecked)
	//		{
	//			// Found a pair
	//			mIndices.push_back(mVertices[i]);
	//			break;
	//		}
	//	}
	//	iterator++;
	//}


	/*sf::Vector2i smallestAngleVertex = mVertices[1];
	float smallestAngle = 3.f;
	bool backAtStart = false;*/

	//while (!backAtStart)
	//{		
	//	// Second to last vertex is current reference
	//	if (mIndices.size() > 1)
	//	{
	//		reference = mVertices.end()[-2];
	//		cur = mVertices.end()[-1];
	//	}

	//	// Find the next vertex with the smallest angle relative to reference
	//	for (int i = 0; i < mVertices.size(); i++)
	//	{
	//		// Skip itself and the reference
	//		if (mVertices[i] == reference || mVertices[i] == cur)
	//			continue;

	//		// Direction from current to reference
	//		sf::Vector2f ref                = sf::Vector2f(reference.x, reference.y);
	//		sf::Vector2f current            = sf::Vector2f(cur.x, cur.y);
	//		sf::Vector2f referenceDirection = ref - current;

	//		// Direction from current to other
	//		sf::Vector2f other              = sf::Vector2f(mVertices[i].x, mVertices[i].y);
	//		sf::Vector2f otherDirection     = other - current;

	//		// Angle between reference and other direction
	//		float testAngle = Toolbox::getAngleBetween(otherDirection, referenceDirection);
	//		if (testAngle < smallestAngle)
	//		{
	//			smallestAngle = testAngle;
	//			smallestAngleVertex = mVertices[i];
	//		}
	//	}
	//	// Save the vertex that had the smallest angle, unless it was the beginning again
	//	if (mVertices[0] != smallestAngleVertex)
	//		mIndices.push_back(smallestAngleVertex);
	//	else
	//		backAtStart = true;
	//}


	/* 
	1. Find the leftmost vertex (minimum x). If there's more than one, choose the lowest of them (minimum y). 
		There is no vertex below the current one, so take the direction 'downwards' as a reference.
	2. Find all edges going from the current vertex and calculate their directions (bearings). 
		Find the one which makes the smallest positive angle (counter-clockwise) from the reference direction. 
		That will be the outline segment.
	3. Select its other end as your new 'current' vertex and set the direction from that vertex 
		to the recent one as a new reference direction.
	4. Proceed from step 2 until you arrive to the start vertex.
	*/

	// #############################################################
	//std::vector<float> vertX;
	//std::vector<float> vertY;

	//std::vector<sf::Vector2i> indices = mIndices;
	//mIndices.clear();

	//for (size_t i = 0; i < indices.size(); i++)
	//{
	//	// Add building middle
	//	vertX.push_back(this->getMiddleOfBuildingGlobal().x);
	//	vertY.push_back(this->getMiddleOfBuildingGlobal().y);

	//	// Add all other points to poly test vector
	//	for (size_t j = 0; j < indices.size(); j++)
	//	{
	//		// Skip itself
	//		if (i != j)
	//		{
	//			sf::Vector2f global = Toolbox::localToGlobalCoords(indices[j]);
	//			sf::Vector2f middle = Toolbox::getMiddleOfBlock(global);
	//			vertX.push_back(middle.x);
	//			vertY.push_back(middle.y);
	//		}
	//	}

	//	// Create test point
	//	sf::Vector2f testPoint = Toolbox::localToGlobalCoords(indices[i]);
	//	sf::Vector2f testPointMiddle = Toolbox::getMiddleOfBlock(testPoint);

	//	// Test if old point is inside polygon. Keep those that are still outside
	//	int inOut = Toolbox::pointInPoly(indices.size(), vertX, vertY, testPointMiddle.x, testPointMiddle.y);
	//	if (!inOut)
	//		mIndices.push_back(indices[i]);

	//	vertX.clear();
	//	vertY.clear();
	//}
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
	std::cout << "point in polygon: " << inside << "\n";

	return inside;
}

void Building::clearPolyPoint()
{
	mIndices.clear();
}


void Building::toggleIndices()
{
	std::cout << "Nr of indices: " << mIndices.size() << "\n";
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
