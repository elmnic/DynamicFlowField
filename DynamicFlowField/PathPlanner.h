#pragma once

#include "FlowGenerator.h"
#include "Map.h"
#include <map>
#include <list>



struct WeightNode 
{
	WeightNode(sf::Vector2i position, int weight, WeightNode* parent = nullptr) {
		this->position = position;
		this->weight = weight;
		this->parent = parent;
	}

	bool visited = false;
	int weight = 0;
	sf::Vector2i position;
	WeightNode* parent;
	std::vector<WeightNode*> children;
};

typedef std::pair<int, int> Point; // Key used in map
typedef std::map<Point, WeightNode*> LookupTable; // Lookup map for finding instance of node
typedef std::list<WeightNode*> Queue; // FIFO queue

class PathPlanner
{
public:
	static PathPlanner* instance();
	~PathPlanner();

	FlowGenerator::FlowField generatePath(sf::Vector2i startPos);

	// Breadth-first-search from startPos
		// Create a field of weights using BFS nodes
	// Pass this field to FlowGenerator
	// Return flow field to agent

private:
	PathPlanner();

	void expandChildren(WeightNode* current);

	Queue mNodeQueue;
	LookupTable mNodeLookup;
	int mIterations;
};

