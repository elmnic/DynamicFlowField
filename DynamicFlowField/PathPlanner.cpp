#include "PathPlanner.h"


PathPlanner* PathPlanner::instance()
{
	static PathPlanner instance;
	return &instance;
}


PathPlanner::PathPlanner()
{
}


PathPlanner::~PathPlanner()
{
}

void PathPlanner::render()
{
	if (!mShortestPath.empty())
	{
		for (auto node : mShortestPath)
		{
			sf::CircleShape circle(Toolbox::getMapBlockSize().x / 4);
			circle.setFillColor(sf::Color::Red);
			circle.setPosition(node->position.x * Toolbox::getMapBlockSize().x, node->position.y * Toolbox::getMapBlockSize().y);
			Toolbox::getWindow().draw(circle);
		}
	}
}

void PathPlanner::clear()
{
	for (auto it : mShortestPath)
		delete it;
	mShortestPath.clear();
}

FlowGenerator::FlowField PathPlanner::generatePath(sf::Vector2i startPos)
{
	// Convert global coords to local index in matrix
	sf::Vector2i localCoordIndex = Toolbox::globalToIndexCoords(startPos);

	// Create start node
	EntityManager::Point pStart(localCoordIndex.x, localCoordIndex.y);

	// Check if startPos is already confirmed. Exit and use pre-existing flow field if true
	Building* testFor = EntityManager::instance()->isBuilding(pStart);
	std::cout << testFor << std::endl;
	if (testFor != nullptr && testFor->getType() == Toolbox::BuildingType::POLYPOINT)
	{
		std::cout << "Start pos was already confirmed \n" << testFor << "\n";
		return FlowGenerator::FlowField();
	}

	// Check if start was on a building. Deny if true
	if (testFor != nullptr && (testFor->getType() == Toolbox::BuildingType::OFFENSIVE || testFor->getType() == Toolbox::BuildingType::DEFENSIVE))
	{
		std::cout << "Start was on building \n";
		return FlowGenerator::FlowField();
	}

	// Add first node to queue
	WeightNode* nStart = new WeightNode(localCoordIndex, 0);
	mNodeLookup[pStart] = nStart;
	mNodeQueue.push_back(nStart);
	nStart->visited = true;

	bool foundBuilding = false;
	mIterations = 0;

	while (!mNodeQueue.empty() && !foundBuilding)
	{
		mIterations++;

		// Retrieve front of queue
		WeightNode* current = mNodeQueue.front();
		mNodeQueue.pop_front();

		// Adds children, if necessary, to queue
		expandChildren(current);

		// Check each child for target and if they've been visited
		for (auto child : current->children)
		{
			EntityManager::Point point(child->position.x, child->position.y);
			Building* target = EntityManager::instance()->isBuilding(point);

			// Skip defensive building
			if (target != nullptr && target->getType() == Toolbox::BuildingType::DEFENSIVE)
				continue;
			
			// If target is on an offensive building
			if (target != nullptr && target->getType() == Toolbox::BuildingType::OFFENSIVE)
			{
				//std::cout << "Found building! " << mIterations << " iterations" << "\n";
				//TODO: Use FlowGenerator to create flow field from building position. Preferably avoiding the other tiles of the building
				foundBuilding = true;
				target->addPolyPoint(localCoordIndex);
				EntityManager::instance()->createConfirmed(localCoordIndex);
				recreatePath(mShortestPath, child);
				break;
			}

			// Add unvisited offensive child to back of queue
			if (!child->visited)
			{
				mNodeQueue.push_back(child);
				child->visited = true;
			}
		}
	}

	// Clear after finishing
	for (std::map<EntityManager::Point, WeightNode*>::iterator it = mNodeLookup.begin(), next_it = mNodeLookup.begin(); it != mNodeLookup.end(); it = next_it)
	{
		next_it = it; ++next_it;
		mNodeLookup.erase(it);
	}
	mNodeLookup.clear();
	mNodeQueue.clear();

	return FlowGenerator::FlowField();
}

void PathPlanner::expandChildren(WeightNode* current)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <=1; j++)
		{
			// Skip itself
			if (i == 0 && j == 0)
				continue;
			// Skip nodes outside horizontally
			if (current->position.x + i < 0 || current->position.x + i > Toolbox::getMapDimensions().x)
				continue;
			// Skip nodes outside vertically
			if (current->position.y + j < 0 || current->position.y + j > Toolbox::getMapDimensions().y)
				continue;

			// Create temp point
			EntityManager::Point p = std::make_pair(current->position.x + i, current->position.y + j);
			LookupTable::iterator search = mNodeLookup.find(p);

			if (search != mNodeLookup.end()) // If point exists
			{
				// If new node has a shorter path and if it is already visited, continue
				WeightNode* node = search->second;
				if (node->weight <= current->weight + 1 && node->visited)
					continue;
				else
				{
					// Current path is shorter. Update weight and parent.
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
}

void PathPlanner::recreatePath(Queue & cameFrom, WeightNode* node)
{
	cameFrom.push_front(node);
	if (node->parent == nullptr)
		return;
	else
		recreatePath(cameFrom, node->parent);
}
