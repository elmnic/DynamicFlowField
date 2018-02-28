#pragma once
#include "Entity.h"

class Building :
	public Entity
{
public:
	Building(int size, Toolbox::BuildingType type, sf::Vector2i pos);
	virtual ~Building();

	virtual void update();
	virtual void render(sf::RenderWindow& window);
	int getSize() { return mSize; }
private:
	int mSize;
	Toolbox::BuildingType mType;
	sf::Vector2i mPosition;
	
	// Info representing how big and what kind of building
	// which is used when rendering to determine which sprite to use
};

