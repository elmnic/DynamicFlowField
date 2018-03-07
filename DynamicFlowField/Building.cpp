#include "Building.h"



Building::Building(int size, Toolbox::BuildingType type, sf::Vector2i pos) :
	mSize(size),
	mType(type),
	mPosition(pos)
{
	// Use mSize to determine which blocks around mPosition will belong to the same building
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
	float sizeX = (float)(window.getSize().x / Toolbox::getMapDimensions().x);
	float sizeY = (float)(window.getSize().y / Toolbox::getMapDimensions().y);

	for (int i = 0; i < mSize; i++)
	{
		for (int j = 0; j < mSize; j++)
		{
			sf::Vector2f realPos = sf::Vector2f((mPosition.x + i) * sizeX, (mPosition.y + j) * sizeY);

			sf::RectangleShape rect(sf::Vector2f(sizeX, sizeY));
			rect.setPosition(realPos);
			rect.setOutlineColor(sf::Color::Black);
			rect.setOutlineThickness(1.f);

			rect.setFillColor(sf::Color::Yellow);
			window.draw(rect);
		}
	}
}
