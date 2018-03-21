#include "PathPlanner.h"


PathPlanner* PathPlanner::instance()
{
	static PathPlanner instance;
	return &instance;
}


PathPlanner::PathPlanner()
{
	mShortestPathTexture.create(Toolbox::getWindow().getSize().x, Toolbox::getWindow().getSize().y);
	mWeightTexture.create(Toolbox::getWindow().getSize().x, Toolbox::getWindow().getSize().y);
}


PathPlanner::~PathPlanner()
{
}

void PathPlanner::render()
{
	sf::Sprite pathSprite(mShortestPathTexture.getTexture());
	Toolbox::getWindow().draw(pathSprite);
	sf::Sprite weightSprite(mWeightTexture.getTexture());
	Toolbox::getWindow().draw(weightSprite);
}

void PathPlanner::clear()
{
	for (auto it : mShortestPath)
		delete it;
	mShortestPath.clear();
	mShortestPathTexture.clear(sf::Color(0, 0, 0, 0));
	mWeightTexture.clear(sf::Color(0, 0, 0, 0));
}

FlowGenerator::FlowField PathPlanner::generatePath(sf::Vector2f startPos)
{
	// Clear Lookup and Queue
	clearNodes();

	// Convert global coords to local coords in matrix
	sf::Vector2i localCoordIndex = Toolbox::globalToIndexCoords(startPos);

	// Create start node
	EntityManager::Point pStart(localCoordIndex.x, localCoordIndex.y);

	// Check if startPos is already confirmed. Exit and use pre-existing flow field if true
	Building* testFor = EntityManager::instance()->isBuilding(pStart);
	if (testFor != nullptr && testFor->getType() == Toolbox::BuildingType::POLYPOINT)
	{
		std::cout << "Start pos was already confirmed\n";
		return FlowGenerator::FlowField();
	}

	// Check if start was on a building. End if true
	if (testFor != nullptr && (testFor->getType() == Toolbox::BuildingType::OFFENSIVE || 
		testFor->getType() == Toolbox::BuildingType::DEFENSIVE))
	{
		std::cout << "Start was on building\n";
		return FlowGenerator::FlowField();
	}

	// Add first node to queue
	WeightNode* nStart = new WeightNode(localCoordIndex, 0);
	mNodeLookup[pStart] = nStart;
	mNodeQueue.push_back(nStart);
	nStart->visited = true;

	// Initial conditions
	bool foundTarget = false;
	mIterations = 0;

	while (!mNodeQueue.empty() && !foundTarget)
	{
		// Counter
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
				//TODO: Use FlowGenerator to create flow field from building position. Preferably avoiding the other tiles of the building
				foundTarget = true;
				// Add point to building to define polygon
				target->addPolyPoint(localCoordIndex);
				EntityManager::instance()->createConfirmed(localCoordIndex);
				recreatePath(mShortestPath, child); // Probably won't need this later

				// Find nearest point on building and generate weights from there
				sf::Vector2i closestPointOnBuilding = target->getPositionClosest(startPos);
				generateWeightMap(closestPointOnBuilding, localCoordIndex);

				// ########### Generate flow field using weight map
				FlowGenerator::instance()->createFlowFieldDynamic(mWeightMap);

				renderToTexture();
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
			if (current->position.x + i < 0 || current->position.x + i > Toolbox::getMapDimensions().x - 1)
				continue;
			// Skip nodes outside vertically
			if (current->position.y + j < 0 || current->position.y + j > Toolbox::getMapDimensions().y - 1)
				continue;

			// Create temp point
			EntityManager::Point p(current->position.x + i, current->position.y + j);
			LookupTable::iterator search = mNodeLookup.find(p);

			// Weight depending on diagonal or horizontal/vertical direction
			float weight = 1.f;
			if (i != 0 && j != 0)
				weight = 1.5f;
			
			if (search != mNodeLookup.end()) // If point exists
			{
				// If new node has a shorter path and if it is already visited, continue
				WeightNode* node = search->second;
				if (node->weight <= current->weight + weight && node->visited)
					continue;
				else
				{
					// Current path is shorter. Update weight and parent.
					node->weight = current->weight + weight;
					node->parent = current;
				}
			}
			else
			{
				// Create new node and add to map and children queue
				WeightNode* node = new WeightNode(sf::Vector2i(p.first, p.second), current->weight + weight, current);
				mNodeLookup[p] = node;
				current->children.push_back(node);
			}
		}
	}
}

// Push parent nodes recursively
void PathPlanner::recreatePath(Queue & cameFrom, WeightNode* node)
{
	cameFrom.push_front(node);
	if (node->parent == nullptr)
		return;
	else
		recreatePath(cameFrom, node->parent);
}

FlowGenerator::WeightMap & PathPlanner::generateWeightMap(sf::Vector2i & startPos, sf::Vector2i & targetPos)
{
	// Initialize weight map
	mWeightMap.clear();
	mWeightMap.resize(Toolbox::getMapDimensions().y, std::vector<float>(Toolbox::getMapDimensions().x, -1.0f));
	clearNodes();
	// Do the same as GeneratePath pretty much.

	// Create start node
	EntityManager::Point pStart(startPos.x, startPos.y);

	// Add first node to queue
	WeightNode* nStart = new WeightNode(startPos, 0);
	mNodeLookup[pStart] = nStart;
	mNodeQueue.push_back(nStart);
	nStart->visited = true;

	// Initial conditions
	bool foundTarget = false;
	mIterations = 0;

	while (!mNodeQueue.empty() && !foundTarget)
	{
		// Counter if needed
		mIterations++;

		// Retrieve front of queue
		WeightNode* current = mNodeQueue.front();
		mNodeQueue.pop_front();

		// Adds children, if necessary, to queue
		expandChildren(current);

		// Check each child for target and if they've been visited
		for (auto child : current->children)
		{
			// Add weight to map
			mWeightMap[child->position.y][child->position.x] = child->weight;

			// Check for building
			EntityManager::Point point(child->position.x, child->position.y);
			Building* target = EntityManager::instance()->isBuilding(point);

			// Skip buildings
			if (target != nullptr && target->getType() == Toolbox::BuildingType::DEFENSIVE || 
				target != nullptr && target->getType() == Toolbox::BuildingType::OFFENSIVE)
				continue;

			// Agent position discovered. Break and return the current weight
			if (child->position == targetPos)
			{
				foundTarget = true;
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
	clearNodes();

	return mWeightMap;
}

void PathPlanner::renderToTexture()
{
	// Render shortest path visualizer to a separate texture for better performance
	/*mShortestPathTexture.clear(sf::Color(0, 0, 0, 0));
	for (auto node : mShortestPath)
	{
		sf::CircleShape circle(Toolbox::getMapBlockSize().x / 4);
		circle.setFillColor(sf::Color::Red);
		circle.setPosition(node->position.x * Toolbox::getMapBlockSize().x, node->position.y * Toolbox::getMapBlockSize().y);
		circle.move(sf::Vector2f(circle.getRadius(), circle.getRadius()));
		mShortestPathTexture.draw(circle);
	}
	mShortestPathTexture.display();*/
	
	// Create text and position it in the middle of the block
	sf::Text text;
	text.setFont(Toolbox::getFont());
	text.setCharacterSize(10);
	mWeightTexture.clear(sf::Color(0, 0, 0, 0));
	for (size_t i = 0; i < mWeightMap.size(); i++)
	{
		for (size_t j = 0; j < mWeightMap[i].size(); j++)
		{
			if (mWeightMap[i][j] >= 0.0f)
			{
				text.setFillColor(sf::Color::Black);
				text.setString(Toolbox::floatToString(mWeightMap[i][j]));
				sf::Vector2i local(j, i);
				sf::Vector2f global = Toolbox::localToGlobalCoords(local);
				text.setPosition(Toolbox::getMiddleOfBlock(global));
				text.move(-sf::Vector2f(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f));
				mWeightTexture.draw(text);
			}
		}
	}
	mWeightTexture.display();

}

void PathPlanner::clearNodes()
{
	// Clear lookup and queue after finishing
		for (std::map<EntityManager::Point, WeightNode*>::iterator it = mNodeLookup.begin(), next_it = mNodeLookup.begin(); it != mNodeLookup.end(); it = next_it)
		{
			next_it = it; ++next_it;
			mNodeLookup.erase(it);
		}
	mNodeLookup.clear();
	mNodeQueue.clear();
}
