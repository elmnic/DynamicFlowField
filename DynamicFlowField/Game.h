#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

#include "Map.h"
#include "GameStates.h"

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

	void printCurrentState();

	sf::RenderWindow mWindow;
	
	StateBase* m_currentState;
};

