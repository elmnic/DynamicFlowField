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
	window.draw(mSprite);
	if (mRenderIndices)
	{
		for (auto it : mIndices)
		{
			sf::CircleShape circle(Toolbox::getMapBlockSize().x / 2);
			circle.setFillColor(sf::Color::Blue);
			circle.setPosition(sf::Vector2f(it.x * Toolbox::getMapBlockSize().x, it.y * Toolbox::getMapBlockSize().y));
			window.draw(circle);
		}
	}
}

void Building::kill()
{
	mAlive = false;
}

// Store polygon indices in global coords
void Building::addPolyPoint(sf::Vector2i point)
{
	mIndices.push_back(point);
}

void Building::clearPolyPoint()
{
	mIndices.clear();
}

void Building::toggleIndices()
{
	std::cout << "Nr of indices: " << mIndices.size() << "\n";
	//mRenderIndices = !mRenderIndices;
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
