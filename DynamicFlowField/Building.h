#pragma once
#include "Entity.h"

#include "LineRenderer.h"

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

	int getSize() { return mSize; }
	Toolbox::BuildingType getType() { return mType; }
	sf::Vector2i& getPosition() { return mPosition; }
	sf::Vector2i getPositionClosest(sf::Vector2f& globalTarget);
	sf::Vector2f getMiddleOfBuildingGlobal();

	// Polygon for confirming flow direction nodes
	void addPolyPoint(sf::Vector2i point);
	void sortPolyPoints();
	void clearPolyPoint();
	std::vector<sf::Vector2i>& getPolyPoints() { return mIndices; }
	
	void toggleIndices();
private:
	void renderToTexture();

	// Convert BuildingType enum to TextureCode enum
	Toolbox::TextureCode buildingToTexture(Toolbox::BuildingType type);

	bool mAlive = true;
	int mSize;
	Toolbox::BuildingType mType;
	sf::Vector2i mPosition;
	
	sf::Sprite mSprite;

	bool mRenderIndices;

	// Index in polygon containing confirmed closest points leading to this building
	std::vector<sf::Vector2i> mIndices;

	std::vector<sf::Vector2f> mClosestPointTargets;


	// Vector containing agent search points acting like corners in a polygon
	// Agent positions become confirmed points 
	// Need at least two agents to create a field of confirmed points

};

