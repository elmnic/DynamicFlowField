#include "PathPlanner.h"


PathPlanner* PathPlanner::instance()
{
	static PathPlanner instance;
	return &instance;
}


PathPlanner::PathPlanner()
{
}

void PathPlanner::expandChildren(WeightNode* current)
{
	for (int i = 0; i < 4; i++)
	{
		int iX, iY;
		switch (i)
		{
		case 0:
			iX = 0;
			iY = -1;
			break;
		case 1:
			iX = 0;
			iY = 1;
			break;
		case 2:
			iX = -1;
			iY = 0;
			break;
		case 3:
			iX = 1;
			iY = 0;
		default:
			break;
		}

		// Create temp point
		EntityManager::Point p = std::make_pair(current->position.x + iX, current->position.y + iY);
		LookupTable::iterator search = mNodeLookup.find(p);

		if (search != mNodeLookup.end()) // If point exists
		{
			// If new node has a shorter path and if it is already visited, continue
			WeightNode* node = search->second;
			if (node->weight < current->weight + 1 && node->visited)
				continue;
			else
			{
				// Current path is shorter. Update weight and parent. (Will probably never happen since it is BFS)
				node->weight = current->weight + 1;
				node->parent = current;
			}
		}
		else
		{
			// Create new node and add to map and children queue
			WeightNode* node = new WeightNode(sf::Vector2i(p.first, p.second), current->weight + 1, current);
			mNodeLookup[p] = node;
			current->children.push_back(node);
		}
	}
}


PathPlanner::~PathPlanner()
{
}

FlowGenerator::FlowField PathPlanner::generatePath(sf::Vector2i startPos)
{
	// Convert global coords to local index in matrix
	sf::Vector2i localCoordIndex = Toolbox::globalToIndexCoords(startPos);

	// Add start node
	EntityManager::Point pStart = std::make_pair(localCoordIndex.x, localCoordIndex.y);
	WeightNode* nStart = new WeightNode(localCoordIndex, 0);
	mNodeLookup[pStart] = nStart;
	mNodeQueue.push_back(nStart);
	nStart->visited = true;

	bool foundBuilding = false;
	mIterations = 0;

	while (!mNodeQueue.empty() && !foundBuilding)
	{
		mIterations++;

		// Dequeue
		WeightNode* current = mNodeQueue.front();
		mNodeQueue.pop_front();

		// Adds children, if necessary, to queue
		expandChildren(current);

		// Check each child for target and if they've been visited
		for (auto child : current->children)
		{
			EntityManager::Point point = std::make_pair(child->position.x, child->position.y);
			Building* target = EntityManager::instance()->isBuilding(point);

			// If child exists and it is on a building
			if (target != nullptr && target->getType() == Toolbox::BuildingType::OFFENSIVE)
			{
				std::cout << "Found building! " << mIterations << " iterations" << "\n";
				foundBuilding = true;
				break;
			}

			// Add unvisited child to back of queue
			if (!child->visited)
			{
				mNodeQueue.push_back(child);
				child->visited = true;
			}
		}
	}

	for (std::map<EntityManager::Point, WeightNode*>::iterator it = mNodeLookup.begin(), next_it = mNodeLookup.begin(); it != mNodeLookup.end(); it = next_it)
	{
		next_it = it; ++next_it;
		mNodeLookup.erase(it);
	}
	mNodeLookup.clear();
	mNodeQueue.clear();

	return FlowGenerator::FlowField();
}
