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
	mRenderClosestPointText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderClosestPointText);
	updateText("Q - Render closest: ", mRenderClosestPointText, Toolbox::getRenderClosestPoints());
	
	mRenderWeightText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderWeightText);
	updateText("W - Render weights: ", mRenderWeightText, Toolbox::getRenderWeights());

	mRenderConfirmedText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderConfirmedText);
	updateText("E - Render confirmed: ", mRenderConfirmedText, Toolbox::getRenderConfirmed());

	mRenderFlowText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderFlowText);
	updateText("F - Render flow: ", mRenderFlowText, Toolbox::getRenderFlow());
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
	if (event.type == sf::Event::Closed || (mWindow->hasFocus() && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
		mWindow->close();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
		game->changeState(EditMap::instance());

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T)
		Toolbox::setTerminateSimulation(false);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
		Toolbox::toggleRenderClosestPoints();
		updateText("Q - Render closests: ", mRenderClosestPointText, Toolbox::getRenderClosestPoints());
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W) {
		Toolbox::toggleRenderWeights();
		updateText("W - Render wights: ", mRenderWeightText, Toolbox::getRenderWeights());
		PathPlanner::instance()->renderWeightToTexture();
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
		Toolbox::toggleRenderConfirmed();
		updateText("E - Render confirmed: ", mRenderConfirmedText, Toolbox::getRenderConfirmed());
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
		Toolbox::toggleRenderTexts();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {
		Toolbox::toggleRenderFlow();
		updateText("F - Render flow: ", mRenderFlowText, Toolbox::getRenderFlow());
		FlowGenerator::instance()->renderFlowToTexture();
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
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
		{
			building->toggleIndices();
			EntityManager::instance()->updateBuildingTexture();
		}

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

void RunGame::updateText(std::string text, sf::Text * sfText, bool status)
{
	std::string statusString = Toolbox::boolToString(status);
	sfText->setFillColor(sf::Color(145, 0, 27)); // Red
	if (status)
		sfText->setFillColor(sf::Color(0, 145, 27)); // Green
	sfText->setString(text + statusString);
}
