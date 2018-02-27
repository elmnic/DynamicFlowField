#pragma once
#include "Entity.h"
class Building :
	public Entity
{
public:
	Building(int size, Map::BuildingType type);
	virtual ~Building();

	virtual void update();
	virtual void render(sf::RenderWindow& window);
	int getSize() { return mSize; }
private:
	int mSize;
	Map::BuildingType mType;
	
	// Info representing how big and what kind of building
	// which is used when rendering to determine which sprite to use
};

