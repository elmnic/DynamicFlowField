#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include "Toolbox.h"
#include "Agent.h"
#include "Building.h"

class EntityManager
{
public:

	static EntityManager* instance();
	~EntityManager();

	void createBuilding(int size, Toolbox::BuildingType type, sf::Vector2i pos);
	void createAgent(sf::Vector2i startPos);
	void update();
	void render(sf::RenderWindow &window);

private:
	EntityManager();

	typedef std::vector<Entity*> EntityVector;
	EntityVector mBuildingVector;
	EntityVector mAgentVector;

};

