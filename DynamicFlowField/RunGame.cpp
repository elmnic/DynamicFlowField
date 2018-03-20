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
	PathPlanner::instance()->render();
	EntityManager::instance()->render(*mWindow);
	TextRenderer::instance()->render();
}

void RunGame::exit()
{
	Map::instance()->unloadMap();
	EntityManager::instance()->exit();
	EntityManager::instance()->clearConfirmed();
	TextRenderer::instance()->clearTextElements();
	PathPlanner::instance()->clear();
}

void RunGame::propagateEvent(Game* game, sf::Event& event)
{
	if (event.type == sf::Event::Closed || (mWindow->hasFocus() && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
		mWindow->close();
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
		game->changeState(EditMap::instance());
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::T)
		Toolbox::setTerminateSimulation(false);
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S)
		Map::instance()->startSimulation();
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
	{ 
		// Clear pathing graphics
		PathPlanner::instance()->clear();
		EntityManager::instance()->clearConfirmed();
	}
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		// Show area of confirmed points from building
		sf::Vector2f mouseCoords((float)event.mouseButton.x, (float)event.mouseButton.y);
		sf::Vector2i localCoords = Toolbox::globalToIndexCoords(mouseCoords);
		EntityManager::Point point(localCoords.x, localCoords.y);
		Building* building = EntityManager::instance()->isBuilding(point);

		// Toggle showing lines
		if (building != nullptr && building->getType() == Toolbox::BuildingType::OFFENSIVE)
			building->toggleIndices();
		PathPlanner::instance()->generatePath(sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y));
	}
}

StateBase::StateID RunGame::getStateID()
{
	return StateBase::StateID::RUNGAME;
}

RunGame::RunGame()
{
}