#pragma once

#include <SFML\Graphics.hpp>
#include "Map.h"
#include "GameState.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	void update();
	void render();

	sf::RenderWindow mWindow;
	

};

