#include "Game.h"

static int MAPSIZE = 30;

Game::Game()
{
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	mWindow.create(sf::VideoMode(mode.height - 200, mode.height - 200), "Dynamic Flow Field", sf::Style::Close);
	mWindow.setFramerateLimit(200);
	mWindow.setKeyRepeatEnabled(false);
	sf::Image icon;
	icon.loadFromFile("resources/icon.png");
	mWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	Toolbox::setWindow(&mWindow);


	// Initial state
	m_currentState = RunGame::instance();
	m_currentState->enter(mWindow);

	// Add fps text
	mFPStext = new sf::Text("-", Toolbox::getFont());
	TextRenderer::instance()->addTextElement(mFPStext);
}


Game::~Game()
{
}


// Main loop
void Game::run()
{
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		mFPSdelay += Toolbox::getDeltaTime().asSeconds();
		Toolbox::setDeltaTime(clock.restart());

		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			// Send event to current game state to handle themselves
			m_currentState->propagateEvent(this, event);
		}
		update();
		render();

		if (mFPSdelay > 0.1f)
		{
			// Render FPS 10 times a second
			mFPSdelay = 0.f;
			float fps = 1.f / Toolbox::getDeltaTime().asSeconds();
			mFPStext->setString("FPS: " + Toolbox::floatToString(std::floor(fps)));
		}
	}
}

// Update current state
void Game::update()
{
	m_currentState->update(this);
}

// Render using current state
void Game::render()
{
	mWindow.clear();
	m_currentState->render();
	mWindow.display();
}

void Game::changeState(StateBase * newState)
{
	// If both states are valid, run exit/enter on corresponding states
	if (m_currentState && newState)
	{
		m_currentState->exit();

		m_currentState = newState;

		// Pass window for easier referencing in states. Deprecated... Window reference is stored in Toolbox
		m_currentState->enter(mWindow);

		// Add FPS text because the text gets deleted when switching state
		mFPStext = new sf::Text("_", Toolbox::getFont());
		TextRenderer::instance()->addTextElement(mFPStext);
	}
}