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

	mRenderWeightText = new sf::Text("Render Weights: " + Toolbox::boolToString(Toolbox::getRenderWeights()), Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderWeightText);

	mRenderClosestPointText = new sf::Text("Render closests: " + Toolbox::boolToString(Toolbox::getRenderClosestPoints()), Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderClosestPointText);

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
	FlowGenerator::instance()->render();
}

void RunGame::exit()
{
	Map::instance()->unloadMap();
	EntityManager::instance()->exit();
	EntityManager::instance()->clearConfirmed();
	TextRenderer::instance()->clearTextElements();
	PathPlanner::instance()->clear();
	FlowGenerator::instance()->clear();
}

void RunGame::propagateEvent(Game* game, sf::Event& event)
{
	if (event.type == sf::Event::Closed || (mWindow->hasFocus() && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
		mWindow->close();
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
		game->changeState(EditMap::instance());
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::T)
		Toolbox::setTerminateSimulation(false);
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::W) {
		Toolbox::toggleRenderWeights(); 
		mRenderWeightText->setString("Render Weights: " + Toolbox::boolToString(Toolbox::getRenderWeights()));
	}
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Q) {
		Toolbox::toggleRenderClosestPoints(); 
		mRenderClosestPointText->setString("Render closests: " + Toolbox::boolToString(Toolbox::getRenderClosestPoints()));
	}
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
		Map::instance()->startSimulation();
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

		// Only spawn on ground and confirmed
		if (building != nullptr && building->getType() == Toolbox::BuildingType::POLYPOINT || 
			building == nullptr)
		{
			EntityManager::instance()->createAgent(localCoords);
		}
	}
}

StateBase::StateID RunGame::getStateID()
{
	return StateBase::StateID::RUNGAME;
}

RunGame::RunGame()
{
}