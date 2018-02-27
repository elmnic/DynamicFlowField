#include "Game.h"

static int MAPSIZE = 30;

Game::Game()
{
	mWindow.create(sf::VideoMode(1000, 1000), "Dynamic Flow Field", sf::Style::Close);

	// Initial state
	m_currentState = RunGame::instance();
	m_currentState->enter(mWindow);
	printCurrentState();
}


Game::~Game()
{
}


// Main loop
void Game::run()
{
	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			// Send event to current game state to handle themselves
			m_currentState->propagateEvent(this, event);
		}
		update();
		render();
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

void Game::printCurrentState()
{
	std::cout << "State: " << m_currentState->getStateID() << "\r";
}

void Game::changeState(StateBase * newState)
{
	// If both states are valid, run exit/enter on corresponding states
	if (m_currentState && newState)
	{
		m_currentState->exit();

		m_currentState = newState;

		// Pass window for easier referencing in states
		m_currentState->enter(mWindow);
	}

	printCurrentState();
}