#include "FlowGenerator.h"



FlowGenerator* FlowGenerator::instance()
{
	static FlowGenerator instance;
	return &instance;
}

FlowGenerator::FlowGenerator()
{
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
