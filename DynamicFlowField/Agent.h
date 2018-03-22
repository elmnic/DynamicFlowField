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

	// Called when target building is lost
	void updatePath();

private:
	bool mAlive = true;

	sf::Vector2f mPosition;
	float mVelocity = 25.f;
	sf::Sprite mSprite;
	FlowGenerator::FlowField mFlowField;
};

