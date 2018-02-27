#pragma once
#include "Entity.h"
class Agent :
	public Entity
{
public:
	Agent();
	virtual ~Agent();

	virtual void update();
	virtual void render(sf::RenderWindow& window);
};

