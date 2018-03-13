#pragma once
#include "Map.h"
#include "TextRenderer.h"
#include <SFML\Graphics.hpp>

class Game;
class StateBase
{
public:
	enum StateID { RUNGAME, EDITMAP };

	StateBase();
	~StateBase();

	// Enter state
	virtual void enter(sf::RenderWindow& window) = 0;

	// Execute state
	virtual void update(Game*) = 0;

	// Render state
	virtual void render() = 0;

	// Exit state
	virtual void exit() = 0;

	// Propagate events forward in case the states need them
	virtual void propagateEvent(Game*, sf::Event& event) = 0;

	// Return the enum for the current state
	virtual StateID getStateID() = 0;

	virtual std::string stateToString(StateID state)
	{
		switch (state)
		{
		case StateBase::RUNGAME:
			return "Run Game";
			break;
		case StateBase::EDITMAP:
			return "Edit Map";
			break;
		default:
			return "Null";
			break;
		}
	}

};

