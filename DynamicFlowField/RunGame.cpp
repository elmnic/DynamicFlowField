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

	mStateText = new sf::Text("Run Game", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mStateText);
}

void RunGame::update(Game* game)
{
	EntityManager::instance()->update();
}

void RunGame::render()
{
	Map::instance()->render(*mWindow);
	EntityManager::instance()->render(*mWindow);
	TextRenderer::instance()->render();
}

void RunGame::exit()
{
	Map::instance()->unloadMap();
	EntityManager::instance()->exit();
	TextRenderer::instance()->clearTextElements();
}

void RunGame::propagateEvent(Game* game, sf::Event& event)
{
	if (event.type == sf::Event::Closed || (mWindow->hasFocus() && event.key.code == sf::Keyboard::Escape))
		mWindow->close();
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
		game->changeState(EditMap::instance());
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::T)
		Toolbox::setTerminateSimulation(false);
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S)
		Map::instance()->startSimulation();
	
}

StateBase::StateID RunGame::getStateID()
{
	return StateBase::StateID::RUNGAME;
}

RunGame::RunGame()
{
}