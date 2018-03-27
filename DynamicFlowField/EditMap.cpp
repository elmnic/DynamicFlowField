#include "EditMap.h"
#include "Game.h"


EditMap * EditMap::instance()
{
	static EditMap instance;
	return &instance;
}

void EditMap::enter(sf::RenderWindow& window)
{
	Toolbox::setFrameRateLocked();
	mWindow = &window;

	Map::instance()->loadMap(Toolbox::LevelCode::LevelDebug);

	mStateText = new sf::Text("Edit Map", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mStateText);

	mScenarioNrText = new sf::Text("Scenario Debug", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mScenarioNrText);

	mCurrentBuildingText = new sf::Text("O-D - Building: Offensive", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mCurrentBuildingText);

	mSizeText = new sf::Text("Z-X - Size:  1", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mSizeText);
}

void EditMap::update(Game* game)
{
	// Mouse input
	// add/delete buildings
	// Save Map
	EntityManager::instance()->update();
}

void EditMap::render()
{
	Map::instance()->render(*mWindow);
	EntityManager::instance()->render(*mWindow);
	TextRenderer::instance()->render();
}

void EditMap::exit()
{
	// Unload map and entities
	Map::instance()->unloadMap();
	EntityManager::instance()->exit();
	TextRenderer::instance()->clearTextElements();
	PathPlanner::instance()->clear();
	FlowGenerator::instance()->clear();
}

void EditMap::propagateEvent(Game* game, sf::Event& event)
{
	if (event.type == sf::Event::Closed || (mWindow->hasFocus() && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
		mWindow->close();

	if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
		changeScenario(event);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
		game->changeState(RunGame::instance());

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O) {
		EntityManager::instance()->setCurrentBuildingType(Toolbox::BuildingType::OFFENSIVE);
		mCurrentBuildingText->setString("O-D - Building: Offensive");
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
		EntityManager::instance()->setCurrentBuildingType(Toolbox::BuildingType::DEFENSIVE);
		mCurrentBuildingText->setString("O-D - Building: Defensive");
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
		EntityManager::instance()->decreaseSize();
		mSizeText->setString("Z-X - Size: " + std::to_string(EntityManager::instance()->getCurrentSize()));
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
		EntityManager::instance()->increaseSize();
		mSizeText->setString("Z-X - Size: " + std::to_string(EntityManager::instance()->getCurrentSize()));
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		// Record agent spawning
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
		// Save map
		Map::instance()->saveMap();
	}

	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
	{
		// Add to map
		sf::Vector2f mouseCoords((float)event.mouseButton.x, (float)event.mouseButton.y);
		sf::Vector2i localCoords = Toolbox::globalToIndexCoords(mouseCoords);
		EntityManager::instance()->editorAddBuilding(localCoords);
	}

	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Right)
	{
		// Add to map
		sf::Vector2f mouseCoords((float)event.mouseButton.x, (float)event.mouseButton.y);
		sf::Vector2i localCoords = Toolbox::globalToIndexCoords(mouseCoords);
		EntityManager::Point point(localCoords.x, localCoords.y);
		Building* building = EntityManager::instance()->isBuilding(point);
		if (building != nullptr)
		{
			building->kill();
			EntityManager::instance()->updateBuildingTexture();
		}
	}
}

void EditMap::changeScenario(sf::Event& event)
{
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

StateBase::StateID EditMap::getStateID()
{
	return StateBase::StateID::EDITMAP;
}

EditMap::EditMap()
{
}