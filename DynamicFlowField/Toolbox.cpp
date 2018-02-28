#include "Toolbox.h"

static sf::Vector2i mMapDimensions;

Toolbox* Toolbox::instance()
{
	Toolbox instance;
	return &instance;
}

void Toolbox::setMapDimensions(sf::Vector2i dim)
{
	mMapDimensions = dim;
}

sf::Vector2i& Toolbox::getMapDimensions()
{
	return mMapDimensions;
}