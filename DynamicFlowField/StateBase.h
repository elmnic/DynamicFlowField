#pragma once
class StateBase
{
public:
	StateBase();
	~StateBase();

	// Enter state
	virtual void enter() = 0;

	// Execute state
	virtual void update() = 0;

	// Exit state
	virtual void exit() = 0;

};

