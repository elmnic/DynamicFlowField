#include "Game.h"

static int MAPSIZE = 30;

Game::Game()
{
	mWindow.create(sf::VideoMode(800, 800), "Dynamic Flow Field", sf::Style::Close);
}


Game::~Game()
{
}


void Game::run()
{

	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.key.code == sf::Keyboard::Escape))
				mWindow.close();
		}
		Game::update();
		Game::render();
	}
}

void Game::update()
{
	// Update entities
	// Do stuff according to input
}

void Game::render()
{
	mWindow.clear();

	//render stuff

	mWindow.display();

}
