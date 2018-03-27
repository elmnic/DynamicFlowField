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

	void updateText(std::string text, sf::Text* sfText, bool status);
	void changeScenario(sf::Event& event);
	void spawnAgent(sf::Vector2f& mouseCoords);

	float mAgentSpawnDelay = 0.f;
	bool mRapidSpawning = false;

	sf::RenderWindow* mWindow;
	sf::Text* mStateText;
	sf::Text* mRenderWeightText;
	sf::Text* mRenderClosestPointText;
	sf::Text* mRenderFlowText;
	sf::Text* mRenderConfirmedText;
	sf::Text* mFlowGenerationText;
	sf::Text* mScenarioNrText;
	sf::Text* mNrOfAgentsText;
};