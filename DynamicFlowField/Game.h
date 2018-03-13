#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

#include "Map.h"
#include "GameStates.h"
#include "EntityManager.h"
#include "TextRenderer.h"

class StateBase;

class Game
{
public:
	Game();
	~Game();
	void run();

	void changeState(StateBase* newState);

private:
	void update();
	void render();


	sf::RenderWindow mWindow;
	StateBase* m_currentState;
};

