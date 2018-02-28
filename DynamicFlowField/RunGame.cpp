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
	std::string test = "lol";
	Map::instance()->loadMap(test);
}

void RunGame::update(Game* game)
{
	// Game loop
}

void RunGame::render()
{
	Map::instance()->render(*mWindow);
	EntityManager::instance()->render(*mWindow);

}

void RunGame::exit()
{
	Map::instance()->unloadMap();
	// Unload entities and map
}

void RunGame::propagateEvent(Game* game, sf::Event& event)
{
	if (event.type == sf::Event::Closed || (mWindow->hasFocus() && event.key.code == sf::Keyboard::Escape))
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