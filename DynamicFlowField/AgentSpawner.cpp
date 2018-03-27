#include "AgentSpawner.h"
#include "EntityManager.h"
#include <chrono>
#include <thread>
#include <iostream>

AgentSpawner* AgentSpawner::instance()
{
	static AgentSpawner instance;
	return &instance;
}


AgentSpawner::AgentSpawner()
{
}

AgentSpawner::~AgentSpawner()
{
	clear();
}

// Increments mTimeElapsed with deltaTime and spawns agents if enough time has passed
void AgentSpawner::update()
{
	if (mRunning)
	{
		// Increment time
		mTimeElapsed += Toolbox::getDeltaTime().asSeconds();

		// Convert mTimeElapsed to string and send it to TextRenderer
		mText->setString("Time elapsed: " + Toolbox::floatToString(mTimeElapsed));

		// Terminate spawning if needed
		if (Toolbox::getTerminateSimulation())
		{
			mAgentQueue.clear();
			mRunning = false;
		}

		if (!mAgentQueue.empty())
		{
			// Retrieve spawn time
			float spawnTime = mAgentQueue.begin()->first;

			// Spawn agent when enough time has passed
			if (mTimeElapsed > spawnTime)
			{
				// Create and remove agent from queue
				EntityManager::instance()->createAgent(mAgentQueue.begin()->second);
				mAgentQueue.erase(mAgentQueue.begin());
			}
		}
	}
}

void AgentSpawner::run()
{
	// Only reset timer if simulation is not already running
	if (!mRunning)
	{
		mText = new sf::Text(sf::String("_"), Toolbox::getFont());
		TextRenderer::instance()->addTextElement(mText);
		mTimeElapsed = 0;
	}
	mRunning = true;
}

void AgentSpawner::clear()
{
	if (!mAgentQueue.empty()) 
		mAgentQueue.clear();
	mRunning = false;
}

void AgentSpawner::queueAgent(sf::Vector2i startCoords, float spawnTime)
{
	mAgentQueue[spawnTime] = startCoords;
}