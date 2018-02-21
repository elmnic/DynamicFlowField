#pragma once
#include "StateBase.h"

class EditMap : public StateBase
{
public:
	static EditMap* instance();
	virtual void enter();
	virtual void update();
	virtual void exit();
private:
	EditMap();
	EditMap(const EditMap&);
	EditMap& operator=(const EditMap&);
};

