#pragma once
#include "Entity.h"
class Building :
	public Entity
{
public:
	Building();
	virtual ~Building();

	virtual void update();
	virtual void render(sf::RenderWindow window);
};

