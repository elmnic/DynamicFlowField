#include "Agent.h"
#include <iostream>


Agent::Agent(sf::Vector2i pos)
{
	// Set texture and scale it to fit in one block
	mSprite.setTexture(Toolbox::getTexture(Toolbox::TextureCode::AGENT));
	float sizeX = Toolbox::getMapBlockSize().x;
	float sizeY = Toolbox::getMapBlockSize().y;

	// How much bigger the texture is compared to the block
	float scaleX = mSprite.getTexture()->getSize().x / sizeX;
	float scaleY = mSprite.getTexture()->getSize().y / sizeY;

	// Reset sprite size to fit block
	mSprite.scale(sf::Vector2f(1 / scaleX, 1 / scaleY));
	mSprite.setPosition(sf::Vector2f((float)pos.x * sizeX, (float)pos.y * sizeY));
}


Agent::~Agent()
{
}

void Agent::update()
{
	// Retrieve velocity from flow field instead
	mSprite.move(mVelocity * Toolbox::getDeltaTime().asSeconds());
}

void Agent::render(sf::RenderWindow& window)
{
	window.draw(mSprite);
}

void Agent::kill()
{
	mAlive = false;
}
