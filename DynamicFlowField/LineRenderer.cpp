#include "LineRenderer.h"


LineRenderer* LineRenderer::instance()
{
	static LineRenderer instance;
	return &instance;
}

LineRenderer::LineRenderer()
{
}


LineRenderer::~LineRenderer()
{
}

void LineRenderer::renderLine(sf::Vector2f start, sf::Vector2f end)
{
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = start;
	line[1].position = end;
	
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Blue;

	Toolbox::getWindow().draw(line);
}
