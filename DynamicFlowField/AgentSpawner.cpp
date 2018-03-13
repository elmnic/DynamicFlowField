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

void AgentSpawner::update()
{
	std::cout << "Time elapsed: " << mTimeElapsed << "\r";
	mTimeElapsed += Toolbox::getDeltaTime().asSeconds();
	if (mRunning)
	{
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
	if (!mRunning)
		mTimeElapsed = 0;
	mRunning = true;
}

void AgentSpawner::clear()
{
	if (!mAgentQueue.empty()) mAgentQueue.clear();
	mRunning = false;
}

void AgentSpawner::queueAgent(sf::Vector2i startCoords, float spawnTime)
{
	mAgentQueue[spawnTime] = startCoords;
}