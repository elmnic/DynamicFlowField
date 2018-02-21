#include "RunGame.h"
#include "Game.h"

RunGame * RunGame::instance()
{
	static RunGame instance;
	return &instance;
}

void RunGame::enter(sf::RenderWindow& window)
{
	mWindow = &window;
	// Load recent map
}

void RunGame::update(Game* game)
{
	// Game loop
}

void RunGame::render()
{

}

void RunGame::exit()
{
	// Unload entities and map
}

void RunGame::propagateEvent(Game* game, sf::Event& event)
{
	if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
		mWindow->close();
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
		game->changeState(EditMap::instance());
}

StateBase::StateID RunGame::getStateID()
{
	return StateBase::StateID::RUNGAME;
}

RunGame::RunGame()
{
}