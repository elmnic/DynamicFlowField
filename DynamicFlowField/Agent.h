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
	virtual const sf::Vector2f& getPosition() { return mSprite.getPosition(); }
	virtual const sf::Sprite& getSprite() { return mSprite; }

	// Called when needing to search for target
	void updatePath();
	void addTarget(const sf::Vector2i target);
	sf::Vector2i getTarget() { return mTarget; }
	void moveAgent(sf::Vector2f& offset);
		
private:
	bool mAlive = true;

	sf::Vector2f             mPosition;
	float                    mSpeed = 40.f;
	sf::Vector2f             mVelocity;
	sf::Sprite               mSprite;
	FlowGenerator::FlowField mFlowField;
	sf::Vector2i             mTarget;
};

