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
	virtual void kill();
	virtual bool isAlive() { return mAlive; }

	int getSize() { return mSize; }
private:
	Toolbox::TextureCode buildingToTexture(Toolbox::BuildingType type);

	bool mAlive = true;
	int mSize;
	Toolbox::BuildingType mType;
	sf::Vector2i mPosition;
	
	sf::Sprite mSprite;
};

