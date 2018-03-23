#pragma once

#include "Entity.h"
#include "FlowGenerator.h"

class Agent :
	public Entity
{
public:
	/// Position in local space
	Agent(sf::Vector2i pos);
	virtual ~Agent();

	virtual void update();
	virtual void render(sf::RenderWindow& window);
	virtual void kill();
	virtual bool isAlive() { return mAlive; }

	// Called when needing to search for target
	void updatePath();

private:
	bool mAlive = true;

	sf::Vector2f             mPosition;
	float                    mSpeed = 25.f;
	sf::Vector2f             mVelocity;
	sf::Sprite               mSprite;
	FlowGenerator::FlowField mFlowField;
};

