#include "Agent.h"
#include "PathPlanner.h"
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

	//Generate path
	updatePath();
}


Agent::~Agent()
{
}

void Agent::update()
{
	if (Toolbox::isBuildingKilled() && !Toolbox::isFinished())
		updatePath();


	// Calculate which block the agent's middle is in
	sf::Vector2f direction;
	sf::Vector2f globalSpriteMid = mSprite.getPosition();
	             globalSpriteMid = Toolbox::getMiddleOfBlock(globalSpriteMid);
	sf::Vector2i localIndex      = Toolbox::globalToIndexCoords(globalSpriteMid);
	sf::Vector2f globalBlockMid  = Toolbox::localToGlobalCoords(localIndex);
	FlowGenerator::Point point(localIndex.x, localIndex.y);
	
	// Get velocity direction from flow field
	if (Toolbox::getDistance(globalSpriteMid, globalBlockMid) <= 20.f)
	{
		mVelocity = mFlowField[point];
	}
	mSprite.move(mVelocity * mSpeed * Toolbox::getDeltaTime().asSeconds());

	// If agent is considered stopped, update path again
	if (mVelocity.x < 0.01f && mVelocity.y < 0.01f && mVelocity.x > -0.01f && mVelocity.y > -0.01f && !Toolbox::isFinished())
	{
		updatePath();
	}
}

void Agent::render(sf::RenderWindow& window)
{
	window.draw(mSprite);
}

void Agent::kill()
{
	mAlive = false;
}

// Called when spawning and when current target is lost
void Agent::updatePath()
{
	sf::Vector2f middle = Toolbox::getMiddleOfBlock(mSprite.getPosition());
	mFlowField = PathPlanner::instance()->generatePath(middle, this);
}

void Agent::addTarget(const sf::Vector2i target)
{
	mTarget = target;
}

void Agent::moveAgent(sf::Vector2f & offset)
{
	mSprite.move(offset);
}
