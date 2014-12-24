#pragma once

#include "LevelEvent.h"

class LevelTrigger {

public:
	LevelEvent* Event;

	virtual void forceTrigger();
};