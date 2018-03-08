#include "Agent.h"
#include <iostream>


Agent::Agent(sf::Vector2i pos)
{
	mPosition = sf::Vector2f(pos.x * Toolbox::getMapBlockSize().x, pos.y * Toolbox::getMapBlockSize().y);
}


Agent::~Agent()
{
}

void Agent::update()
{
	mPosition += mVelocity * Toolbox::getDeltaTime().asSeconds();
}

void Agent::render(sf::RenderWindow& window)
{
	// Get the size of a block on the map
	sf::Vector2f size = Toolbox::getMapBlockSize();

	// Create a circle with diameter of block size
	sf::CircleShape circle(size.x / 2);
	circle.setPosition(mPosition);
	circle.setOutlineColor(sf::Color::Black);
	circle.setOutlineThickness(1.f);

	circle.setFillColor(sf::Color::White);
	window.draw(circle);
}

void Agent::kill()
{
	mAlive = false;
}
