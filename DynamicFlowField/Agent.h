#pragma once
#include "Entity.h"
class Agent :
	public Entity
{
public:
	Agent(sf::Vector2i pos);
	virtual ~Agent();

	virtual void update();
	virtual void render(sf::RenderWindow& window);
	virtual void kill();
	virtual bool isAlive() { return mAlive; }
private:
	bool mAlive = true;

	sf::Vector2f mPosition;
	sf::Vector2f mVelocity = sf::Vector2f(40.f, 40.f);
};

