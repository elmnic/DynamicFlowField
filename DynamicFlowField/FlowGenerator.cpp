#include "FlowGenerator.h"
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


FlowGenerator::WeightMap & FlowGenerator::generateWeightMap(sf::Vector2i startPos)
{
	// TODO: insert return statement here
	return mPersonalWeightMap;

}


FlowGenerator::~FlowGenerator()
{
}

void FlowGenerator::render()
{
	sf::Sprite flow(mFlowTexture.getTexture());
	Toolbox::getWindow().draw(flow);
}

void FlowGenerator::clear()
{
	mFlowTexture.clear(sf::Color(0, 0, 0, 0));

	mSharedFlowField.clear();

	mPersonalFlowField.clear();
}

FlowGenerator::FlowField & FlowGenerator::createFlowFieldStatic(WeightMap& weights)
{
	FlowField personalFlowField;

	// TODO: generate flow field

	return personalFlowField;

}

FlowGenerator::FlowField & FlowGenerator::createFlowFieldDynamic(WeightMap& weights)
{
	// Iterate through all weights
	for (auto node : weights)
	{
		// Create easier to read vector using the pair of ints in map
		int col = node.first.first;  // X
		int row = node.first.second; // Y
		sf::Vector2i nodeLocal(col, row);
		sf::Vector2i lowestWeight(row, col);

		// Iterate through all directions around a certain weight to find the cheapest point
		for (int vertical = -1; vertical <= 1; vertical++)
		{
			for (int horizontal = -1; horizontal <= -1; horizontal++)
			{
				// Skip itself
				if (vertical == 0 && horizontal == 0)
					continue;

				// Skip position outside horizontally/vertically
				if (row + vertical < 0 || row + vertical > Toolbox::getMapDimensions().y - 1 ||
					col + horizontal < 0 || col + horizontal > Toolbox::getMapDimensions().x - 1)
					continue;

				// Find the position with the lowest weight
				FlowGenerator::Point testPoint(col + horizontal, row + vertical);
				if (weights.find(testPoint) != weights.end())
				{
					FlowGenerator::Point lowestWeightPoint(lowestWeight.x, lowestWeight.y);
					if (weights[testPoint] < weights[lowestWeightPoint])
						lowestWeight = sf::Vector2i(testPoint.second, testPoint.first);
				}
			}
		}
		// Calculate direction to lowest weight node
		sf::Vector2f direction;
		sf::Vector2f globalOrigin = Toolbox::localToGlobalCoords(nodeLocal);
		             globalOrigin = Toolbox::getMiddleOfBlock(globalOrigin);
		sf::Vector2f globalA      = Toolbox::localToGlobalCoords(lowestWeight);
		             globalA      = Toolbox::getMiddleOfBlock(globalA);

		direction = globalA - globalOrigin;

		// Set the direction vector at the correct position
		mSharedFlowField[node.first] = Toolbox::normalize(direction);
	}

	renderToTexture();

	return mSharedFlowField;

	//for (int row = 0; row < weights.size(); row++)
	//{
	//	for (int col = 0; col < weights[row].size(); col++)
	//	{

	//		// Skip uninitialized locations
	//		if (weights[col][row] < 0.0f)
	//			continue;

	//		sf::Vector2i localOrigin(row, col);
	//		sf::Vector2i lowestW1(row, col);

	//		// Iterate through all directions around a certain weight to find the cheapest point
	//		for (int vertical = -1; vertical <= 1; vertical++)
	//		{
	//			for (int horizontal = -1; horizontal <= -1; horizontal++)
	//			{
	//				// Skip itself
	//				if (vertical == 0 && horizontal == 0)
	//					continue;

	//				// Skip position outside horizontally/vertically
	//				if (row + vertical < 0 || row + vertical > Toolbox::getMapDimensions().y - 1 ||
	//					col + horizontal < 0 || col + horizontal > Toolbox::getMapDimensions().x - 1)
	//					continue;

	//				// Find the position with the lowest weight
	//				if (weights[col + horizontal][row + vertical] < weights[lowestW1.y][lowestW1.x] && weights[col + horizontal][row + vertical] >= 0.0f)
	//					lowestW1 = sf::Vector2i(row + vertical, col + horizontal);
	//			}
	//		}

	//		// Calculate direction to lowest weight node
	//		sf::Vector2f direction;
	//		FlowGenerator::Point point(localOrigin.x, localOrigin.y);
	//		sf::Vector2f globalOrigin = Toolbox::localToGlobalCoords(localOrigin);
	//		             globalOrigin = Toolbox::getMiddleOfBlock(globalOrigin);
	//		sf::Vector2f globalA      = Toolbox::localToGlobalCoords(lowestW1);
	//		             globalA      = Toolbox::getMiddleOfBlock(globalA);

	//		direction = globalA - globalOrigin;

	//		mSharedFlowField[point] = Toolbox::normalize(direction);
	//	}
	//}

}

void FlowGenerator::renderToTexture()
{
	mFlowTexture.clear(sf::Color(0, 0, 0, 0));
	for (FlowGenerator::FlowField::iterator it = mSharedFlowField.begin(); it != mSharedFlowField.end(); it++)
	{
		sf::Vector2i localStart(it->first.first, it->first.second);
		
		sf::Vector2f globalStart = Toolbox::localToGlobalCoords(localStart);
			            globalStart = Toolbox::getMiddleOfBlock(globalStart);
		sf::Vector2f globalEnd   = globalStart + it->second;
			            globalEnd   = Toolbox::getMiddleOfBlock(globalEnd);
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = globalStart;
		line[1].position = globalEnd;

		line[0].color = sf::Color::Red;
		line[1].color = sf::Color::Blue;

		mFlowTexture.draw(line);
	}
	mFlowTexture.display();
}

