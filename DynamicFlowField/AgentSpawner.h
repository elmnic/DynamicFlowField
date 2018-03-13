#pragma once

#include "EntityManager.h"
#include "TextRenderer.h"

class AgentSpawner
{
public:
	static AgentSpawner* instance();
	~AgentSpawner();

	void update();
	void queueAgent(sf::Vector2i startCoords, float spawnTime);

	// Initiate spawning
	void run();

	void clear();

private:
	AgentSpawner();

	// Map start coordinates to a spawn timer
	std::map<float, sf::Vector2i> mAgentQueue;

	float mTimeElapsed = 0.0f;

	bool mRunning = false;

	sf::Text* mText;
};

