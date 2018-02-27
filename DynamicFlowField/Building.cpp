#include "Building.h"



Building::Building(int size, Map::BuildingType type) :
	mSize(size), 
	mType(type)
{

}


Building::~Building()
{
}

void Building::update()
{
	// Check if dead
}

void Building::render(sf::RenderWindow& window)
{
	// Get size and sprite/color via map
}
