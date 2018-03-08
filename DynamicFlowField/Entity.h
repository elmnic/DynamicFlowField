#pragma once

#include <SFML\Graphics.hpp>
#include "Toolbox.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void update() = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual void kill() = 0;
	virtual bool isAlive() = 0;
};

