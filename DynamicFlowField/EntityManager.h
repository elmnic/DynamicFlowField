#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include <map>

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

	struct BuildingRegion
	{
		BuildingRegion(int size, sf::Vector2i pos)
		{
			xMin = pos.x;
			xMax = xMin + size;
			yMin = pos.y;
			yMax = yMin + size;
		}

		int xMin;
		int yMin;
		int xMax;
		int yMax;

		// Helper method to determine if a point is in region
		bool isInRegion(int x, int y)
		{
			return xMin <= x && x <= xMax && yMin <= y && y <= yMax;
		}

		bool operator<(const BuildingRegion &right) const
		{
			if (xMin == right.xMin)
			{
				if (yMin == right.yMin)
				{
					if (xMax == right.xMax)
					{
						return yMax < right.yMax;
					}
					else
					{
						return xMax < right.xMax;
					}
				}
				else
				{
					return yMin < right.yMin;
				}
			}
			else
			{
				return xMin < right.xMin;
			}
		}
	};

	// Entities
	typedef std::vector<Entity*> EntityVector;
	EntityVector mBuildings;
	EntityVector mAgents;

	typedef std::vector<BuildingRegion*> RegionVector;
	RegionVector mRegions;

	// Map coordinate regions to specific building
	std::map<BuildingRegion*, Building*> mBuildingMap;
};

