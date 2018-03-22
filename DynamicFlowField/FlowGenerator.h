#pragma once

#include <SFML\System\Vector2.hpp>
#include <vector>
#include <map>
#include "Toolbox.h"

class FlowGenerator
{
public:
	// 2D vector with directions
	typedef std::pair<int, int> Point;
	typedef std::map<Point, sf::Vector2f> FlowField;
	typedef std::map<Point, float> WeightMap;
	
	static FlowGenerator* instance();
	~FlowGenerator();

	/*TODO: Use generatePath from PathPlanner but from the target building position until the agent's position is found.
		Store the generated weights and compute a direction field and either store or just return the field, 
		depending on static or dynamic*/
	// Non dynamic flow field
	FlowField& createFlowFieldStatic(WeightMap& weights);

	// Dynamic flow field. Will update a shared field used by all agents
	FlowField& createFlowFieldDynamic(WeightMap& weights);

	void render();

	void clear();

private:
	FlowGenerator();
	void renderToTexture();

	WeightMap& generateWeightMap(sf::Vector2i startPos);

	WeightMap mSharedWeightMap;
	FlowField mSharedFlowField;

	WeightMap mPersonalWeightMap;
	FlowField mPersonalFlowField;

	sf::RenderTexture mFlowTexture;
};

