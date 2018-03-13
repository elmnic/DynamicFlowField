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
	mSprite.scale(sf::Vector2f(1/scaleX, 1/scaleY));

	// Scale sprite to accomodate building size 
	mSprite.scale(sf::Vector2f(size, size));
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
	//for (int i = 0; i < mSize; i++)
	//{
	//	for (int j = 0; j < mSize; j++)
	//	{
	//		sf::Vector2f realPos = sf::Vector2f((mPosition.x + i) * sizeX, (mPosition.y + j) * sizeY);

	//		sf::RectangleShape rect(sf::Vector2f(sizeX, sizeY));
	//		rect.setPosition(realPos);
	//		rect.setOutlineColor(sf::Color::Black);
	//		rect.setOutlineThickness(1.f);

	//		switch (mType)
	//		{
	//		case Toolbox::OFFENSIVE:
	//			rect.setFillColor(sf::Color::Yellow);
	//			break;
	//		case Toolbox::DEFENSIVE:
	//			rect.setFillColor(sf::Color(50, 50, 50)); // Gray
	//			break;
	//		default:
	//			break;
	//		}

	//		window.draw(rect);
	//	}
	//}
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
