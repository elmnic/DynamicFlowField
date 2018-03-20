#pragma once

#include "Toolbox.h"

class LineRenderer
{
public:
	static LineRenderer* instance();
	~LineRenderer();

	void renderLine(sf::Vector2f start, sf::Vector2f end);

private:
	LineRenderer();
};

