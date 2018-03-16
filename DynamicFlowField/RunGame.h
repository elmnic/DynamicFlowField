#pragma once
#include "StateBase.h"

#include "PathPlanner.h"

class RunGame : public StateBase
{
public:
	static RunGame* instance();
	virtual void enter(sf::RenderWindow& window);
	virtual void update(Game* game);
	virtual void render();
	virtual void exit();
	virtual void propagateEvent(Game* game, sf::Event& event);
	virtual StateID getStateID();
private:
	RunGame();
	RunGame(const RunGame&);
	RunGame& operator=(const RunGame&);

	sf::RenderWindow* mWindow;
	sf::Text* mStateText;
};