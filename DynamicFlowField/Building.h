#pragma once
#include "Entity.h"
#include "LineRenderer.h"
#include <algorithm>

class Building :
	public Entity
{
public:
	Building(int size, Toolbox::BuildingType type, sf::Vector2i pos);
	virtual ~Building();

	virtual void update();
	virtual void render(sf::RenderWindow& window);
	virtual void kill();
	virtual bool isAlive() { return mAlive; }

	//------------------- Building specific functions

	sf::Sprite& getSprite() { return mSprite; }
	int getSize() { return mSize; }
	Toolbox::BuildingType getType() { return mType; }
	sf::Vector2i& getPosition() { return mPosition; }
	sf::Vector2i getPositionClosest(sf::Vector2f& globalTarget);
	sf::Vector2f getMiddleOfBuildingGlobal();

	// Polygon for confirming flow direction nodes
	bool addPolyPoint(sf::Vector2i point);
	void sortPolyPoints();
	bool isPointInPoly(sf::Vector2i& point);
	void clearPolyPoint();
	
	void toggleIndices();
private:

	struct Point
	{
		sf::Vector2i coords;
		bool operator < (const Point& p) const
		{
			return coords.x < p.coords.x || (coords.x == p.coords.x && coords.y < p.coords.y);
		}
	};

	int cross(const Point& O, const Point &A, const Point &B);
	void convex_hull();

	// Convert BuildingType enum to TextureCode enum
	Toolbox::TextureCode buildingToTexture(Toolbox::BuildingType type);

	bool                  mAlive = true;
	int                   mSize;
	Toolbox::BuildingType mType;
	sf::Vector2i          mPosition;
	sf::Sprite            mSprite;
	bool                  mRenderIndices;

	// Index in polygon containing confirmed closest points leading to this building
	std::vector<Point> mIndices;

	std::vector<sf::Vector2f> mClosestPointTargets;


	// Vector containing agent search points acting like corners in a polygon
	// Agent positions become confirmed points 
	// Need at least two agents to create a field of confirmed points

};

