#include "EditMap.h"
#include "Game.h"


EditMap * EditMap::instance()
{
	static EditMap instance;
	return &instance;
}

void EditMap::enter(sf::RenderWindow& window)
{
	mWindow = &window;
}

void EditMap::update(Game* game)
{
	// Mouse input
	// add/delete buildings
	// Save Map
}

void EditMap::render()
{

}

void EditMap::exit()
{
	// Unload map and entities
}

void EditMap::propagateEvent(Game* game, sf::Event& event)
{
	if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
		mWindow->close();
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
		game->changeState(RunGame::instance());
}

StateBase::StateID EditMap::getStateID()
{
	return StateBase::StateID::EDITMAP;
}

EditMap::EditMap()
{
}