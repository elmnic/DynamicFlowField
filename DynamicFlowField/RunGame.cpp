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
	Map::instance()->loadMap(Toolbox::LevelCode::LevelDebug);

	mStateText = new sf::Text("Run Game", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mStateText);

	// Set initial text and color depending on the boolean value
	mScenarioNrText = new sf::Text("Scenario Debug", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mScenarioNrText);

	mNrOfAgentsText = new sf::Text("Agents: ", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mNrOfAgentsText);

	mRenderClosestPointText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderClosestPointText);
	updateText("Q - Render closest: ", mRenderClosestPointText, Toolbox::getRenderClosestPoints());
	
	mRenderWeightText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderWeightText);
	updateText("W - Render weights: ", mRenderWeightText, Toolbox::getRenderWeights());

	mRenderConfirmedText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderConfirmedText);
	updateText("E - Render confirmed: ", mRenderConfirmedText, Toolbox::getRenderConfirmed());

	mFlowGenerationText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mFlowGenerationText);
	updateText("S - Dynamic flow generation: ", mFlowGenerationText, Toolbox::getGenerateDynamicFlow());

	mRenderFlowText = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mRenderFlowText);
	updateText("F - Render flow: ", mRenderFlowText, Toolbox::getRenderFlow());
}

void RunGame::update(Game* game)
{
	if (!Toolbox::isFinished())
	{
		mAgentSpawnDelay += Toolbox::getDeltaTime().asSeconds();
		PathPlanner::instance()->update();
		EntityManager::instance()->update();
	}
	
	mNrOfAgentsText->setString("Agents: " + std::to_string(EntityManager::instance()->getNrOfAgents()));
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
	TextRenderer::instance()->clearTextElements();
	PathPlanner::instance()->clear();
	FlowGenerator::instance()->clear();
}

void RunGame::propagateEvent(Game* game, sf::Event& event)
{
	if (event.type == sf::Event::Closed || (mWindow->hasFocus() && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
		mWindow->close();

	if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
		changeScenario(event);

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

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
		Toolbox::toggleGenerateDynamicFlow();
		updateText("S - Dynamic flow generation: ", mFlowGenerationText, Toolbox::getGenerateDynamicFlow());
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
		Toolbox::toggleRenderTexts();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {
		Toolbox::toggleRenderFlow();
		updateText("F - Render flow: ", mRenderFlowText, Toolbox::getRenderFlow());
		FlowGenerator::instance()->renderFlowToTexture();
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
		Toolbox::toggleFrameRateLocked();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		Toolbox::setIsFinished(false);
		Map::instance()->startSimulation();
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
		{
			building->toggleIndices();
			EntityManager::instance()->updateBuildingTexture();
		}

		spawnAgent(mouseCoords);
	}

	//############################################### Spara undan musens position möjligtvis
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
		mRapidSpawning = true;
		
		if (mRapidSpawning && mAgentSpawnDelay > 0.15f)
		{
			mAgentSpawnDelay = 0.f;
			sf::Vector2f mouseCoords((float)event.mouseButton.x, (float)event.mouseButton.y);
			spawnAgent(mouseCoords);
		}
	}

	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
		mRapidSpawning = false;
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

void RunGame::changeScenario(sf::Event& event)
{
	Map::instance()->unloadMap();
	Toolbox::LevelCode scenario;
	switch (event.key.code)
	{
	case sf::Keyboard::Num0:
		scenario = Toolbox::LevelCode::LevelDebug;
		mScenarioNrText->setString("Scenario Debug");
		break;
	case sf::Keyboard::Num1:
		scenario = Toolbox::LevelCode::Level1;
		mScenarioNrText->setString("Scenario 1");
		break;
	case sf::Keyboard::Num2:
		scenario = Toolbox::LevelCode::Level2;
		mScenarioNrText->setString("Scenario 2");
		break;
	case sf::Keyboard::Num3:
		scenario = Toolbox::LevelCode::Level3;
		mScenarioNrText->setString("Scenario 3");
		break;
	case sf::Keyboard::Num4:
		scenario = Toolbox::LevelCode::Level4;
		mScenarioNrText->setString("Scenario 4");
		break;
	case sf::Keyboard::Num5:
		scenario = Toolbox::LevelCode::Level5;
		mScenarioNrText->setString("Scenario 5");
		break;
	case sf::Keyboard::Num6:
		scenario = Toolbox::LevelCode::Level6;
		mScenarioNrText->setString("Scenario 6");
		break;
	case sf::Keyboard::Num7:
		scenario = Toolbox::LevelCode::Level7;
		mScenarioNrText->setString("Scenario 7");
		break;
	case sf::Keyboard::Num8:
		scenario = Toolbox::LevelCode::Level8;
		mScenarioNrText->setString("Scenario 8");
		break;
	case sf::Keyboard::Num9:
		scenario = Toolbox::LevelCode::Level9;
		mScenarioNrText->setString("Scenario 9");
		break;
	default:
		scenario = Toolbox::LevelCode::LevelDebug;
		mScenarioNrText->setString("Scenario Debug");
		break;
	}
	Toolbox::setIsFinished(false);
	Map::instance()->loadMap(scenario);
}

void RunGame::spawnAgent(sf::Vector2f & mouseCoords)
{
	sf::Vector2i localCoords = Toolbox::globalToIndexCoords(mouseCoords);
	EntityManager::Point point(localCoords.x, localCoords.y);
	Building* building = EntityManager::instance()->isBuilding(point);
	// Only spawn on ground and confirmed
	if (building != nullptr && building->getType() == Toolbox::BuildingType::POLYPOINT ||
		building == nullptr)
	{
		EntityManager::instance()->createAgent(localCoords);
	}
}
