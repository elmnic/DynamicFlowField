#pragma once

#include <SFML\System\Vector2.hpp>
#include <vector>

class FlowGenerator
{
public:
	// 2D vector with directions
	typedef std::vector<std::vector<sf::Vector2f>> FlowField;
	typedef std::vector<std::vector<int>> WeightMap;
	
	static FlowGenerator* instance();
	~FlowGenerator();

	// Non dynamic flow field
	FlowField& createFlowFieldStatic(WeightMap weights);

	// Dynamic flow field
	FlowField& createFlowFieldDynamic(WeightMap weights);

private:
	FlowGenerator();

	WeightMap mAccumulatedWeights;
	FlowField mSharedFlowField;

};

