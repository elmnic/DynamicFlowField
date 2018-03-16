#include "Building.h"

#include <iostream>

Building::Building(int size, Toolbox::BuildingType type, sf::Vector2i pos) :
	mSize(size),
	mType(type),
	mPosition(pos)
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
	window.draw(mSprite);
}

void Building::kill()
{
	mAlive = false;
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
	default:
		break;
	}
	return Toolbox::TextureCode();
}
