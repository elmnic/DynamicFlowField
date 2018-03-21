#include "FlowGenerator.h"
#include <iostream>


FlowGenerator* FlowGenerator::instance()
{
	static FlowGenerator instance;
	return &instance;
}

FlowGenerator::FlowGenerator()
{
}

FlowGenerator::WeightMap & FlowGenerator::generateWeightMap(sf::Vector2i startPos)
{
	// TODO: insert return statement here
	return mPersonalWeightMap;

}


FlowGenerator::~FlowGenerator()
{
}

FlowGenerator::FlowField & FlowGenerator::createFlowFieldStatic(WeightMap weights)
{
	FlowField personalFlowField;

	// TODO: generate flow field

	return personalFlowField;

}

FlowGenerator::FlowField & FlowGenerator::createFlowFieldDynamic(WeightMap weights)
{

	// TODO: generate flow field


	return mSharedFlowField;
}
