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

	std::string truefalse;

	// Set initial text and color depending on the boolean value
	truefalse = Toolbox::boolToString(Toolbox::getRenderWeights());
	mRenderWeightText = new sf::Text("Render weights: " + truefalse, Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderWeightText);
	mRenderWeightText->setFillColor(sf::Color(145, 0, 27));
	if (truefalse == "true")
		mRenderWeightText->setFillColor(sf::Color(0, 145, 27));

	truefalse = Toolbox::boolToString(Toolbox::getRenderClosestPoints());
	mRenderClosestPointText = new sf::Text("Render closests: " + truefalse, Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderClosestPointText);
	mRenderClosestPointText->setFillColor(sf::Color(145, 0, 27));
	if (truefalse == "true")
		mRenderClosestPointText->setFillColor(sf::Color(0, 145, 27));

	truefalse = Toolbox::boolToString(Toolbox::getRenderFlow());
	mRenderFlowText = new sf::Text("Render flow: " + truefalse, Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderFlowText);
	mRenderFlowText->setFillColor(sf::Color(145, 0, 27));
	if (truefalse == "true")
		mRenderFlowText->setFillColor(sf::Color(0, 145, 27));
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
	FlowGenerator::instance()->render();
	PathPlanner::instance()->render();
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
		std::string truefalse = Toolbox::boolToString(Toolbox::getRenderWeights());
		mRenderWeightText->setFillColor(sf::Color(145, 0, 27));
		if (truefalse == "true")
			mRenderWeightText->setFillColor(sf::Color(0, 145, 27));
		mRenderWeightText->setString("Render wights: " + truefalse);
	}

	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Q) {
		Toolbox::toggleRenderClosestPoints(); 
		std::string truefalse = Toolbox::boolToString(Toolbox::getRenderClosestPoints());
		mRenderClosestPointText->setFillColor(sf::Color(145, 0, 27));
		if (truefalse == "true")
			mRenderClosestPointText->setFillColor(sf::Color(0, 145, 27));
		mRenderClosestPointText->setString("Render closests: " + truefalse);
	}

	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F) {
		Toolbox::toggleRenderFlow();
		std::string truefalse = Toolbox::boolToString(Toolbox::getRenderFlow());
		mRenderFlowText->setFillColor(sf::Color(145, 0, 27));
		if (truefalse == "true")
			mRenderFlowText->setFillColor(sf::Color(0, 145, 27));
		mRenderFlowText->setString("Render flow: " + truefalse);
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