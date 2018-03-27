#include "FlowGenerator.h"
#include "EntityManager.h"
#include <iostream>


FlowGenerator* FlowGenerator::instance()
{
	static FlowGenerator instance;
	return &instance;
}

FlowGenerator::FlowGenerator()
{
	clear();
	mFlowTexture.create(Toolbox::getWindow().getSize().x, Toolbox::getWindow().getSize().y);
}

FlowGenerator::FlowField & FlowGenerator::generateFlowField(WeightMap & weights, FlowField& workField)
{
	// Iterate through all weights
	for (auto node : weights)
	{
		bool nearbyWall = false;

		// Create easier to read vector using the pair of ints in map
		int col = node.first.first;  // X
		int row = node.first.second; // Y
		sf::Vector2i nodeLocal(col, row);
		FlowGenerator::Point currentLowestPoint(nodeLocal.x, nodeLocal.y);

		// Find a valid default point
		for (int vertical = -1; vertical <= 1; vertical++)
		{
			for (int horizontal = -1; horizontal <= 1; horizontal++)
			{
				FlowGenerator::Point test(col + horizontal, row + vertical);
				Building* building = EntityManager::instance()->isBuilding(test);
				if (building != nullptr && building->getType() == Toolbox::BuildingType::DEFENSIVE)
					nearbyWall = true;

				/* Limit check to only horizontal/vertical if there is a wall nearby.
				*  This prevents agents from making turns to early and ending up stuck inside a wall
				*/
				if (nearbyWall)
				{
					if (vertical == 0 || horizontal == 0)
					{
						if (weights.find(test) != weights.end())
							currentLowestPoint = test;
					}
				}
				else
				{
					if (weights.find(test) != weights.end())
						currentLowestPoint = test;
				}
			}
		}

		// Iterate through all directions around a certain weight to find the cheapest point
		for (int vertical = -1; vertical <= 1; vertical++)
		{
			for (int horizontal = -1; horizontal <= 1; horizontal++)
			{
				// Skip itself
				if (vertical == 0 && horizontal == 0)
					continue;
				// Skip diagonal if wall is nearby
				if (nearbyWall)
					if (vertical != 0 && horizontal != 0)
						continue;

				// Find the position with the lowest weight
				FlowGenerator::Point testPoint(col + horizontal, row + vertical);

				// If the point has weight
				if (weights.find(testPoint) != weights.end())
				{
					if (weights.find(testPoint)->second <= weights.find(currentLowestPoint)->second)
						currentLowestPoint = testPoint;
				}
			}
		}
		// Skip adding direction vector if the current node is on a building point with 0 weight
		if (weights[node.first] == 0)
			continue;

		// Calculate direction to lowest weight node
		sf::Vector2i lowestWeight(currentLowestPoint.first, currentLowestPoint.second);
		sf::Vector2f direction;
		sf::Vector2f globalOrigin = Toolbox::localToGlobalCoords(nodeLocal);
		globalOrigin = Toolbox::getMiddleOfBlock(globalOrigin);
		sf::Vector2f globalA = Toolbox::localToGlobalCoords(lowestWeight);
		globalA = Toolbox::getMiddleOfBlock(globalA);

		direction = globalA - globalOrigin;

		// Set the direction vector at the correct position
		sf::Vector2f normalized = Toolbox::normalize(direction);
		workField[node.first] = normalized;
	}

	return workField;
}

FlowGenerator::~FlowGenerator()
{
}

void FlowGenerator::render()
{
	if (Toolbox::getRenderFlow())
	{
		sf::Sprite flow(mFlowTexture.getTexture());
		Toolbox::getWindow().draw(flow);
	}
}

void FlowGenerator::clear()
{
	mFlowTexture.clear(sf::Color(0, 0, 0, 0));
	renderFlowToTexture();

	mSharedFlowField.clear();

	mPersonalFlowField.clear();
}

// Clear the flow field for each agent, making them generate a completely new one
FlowGenerator::FlowField & FlowGenerator::createFlowFieldStatic(WeightMap& weights)
{
	// Clear between runs
	mPersonalFlowField.clear();

	// Generate a flow field
	generateFlowField(weights, mPersonalFlowField);

	// Generate flow, if enabled, to a texture
	renderFlowToTexture();

	return mPersonalFlowField;

}

// Used the shared field and add to it if neccessary
FlowGenerator::FlowField & FlowGenerator::createFlowFieldDynamic(WeightMap& weights)
{
	// Generate a flow field
	generateFlowField(weights, mSharedFlowField);

	// Generate flow, if enabled, to a texture
	renderFlowToTexture();

	return mSharedFlowField;
}

void FlowGenerator::renderFlowToTexture()
{
	mFlowTexture.clear(sf::Color(0, 0, 0, 0));
	if (Toolbox::getRenderFlow())
	{
		FlowField& currentField = Toolbox::getGenerateDynamicFlow() ? mSharedFlowField : mPersonalFlowField;
		for (FlowGenerator::FlowField::iterator it = currentField.begin(); it != currentField.end(); it++)
		{
			sf::Vector2i localStart(it->first.first, it->first.second);

			sf::Vector2f globalStart = Toolbox::localToGlobalCoords(localStart);
			globalStart = Toolbox::getMiddleOfBlock(globalStart);
			sf::Vector2f globalEnd = globalStart + it->second * Toolbox::getMapBlockSize().x * 0.5f;
			sf::VertexArray line(sf::Lines, 2);
			line[0].position = globalStart;
			line[1].position = globalEnd;

			line[0].color = sf::Color::Red;
			line[1].color = sf::Color::Blue;

			mFlowTexture.draw(line);
		}
	}
	mFlowTexture.display();
}

