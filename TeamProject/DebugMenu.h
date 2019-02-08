#pragma once
#include "GameTechRenderer.h"

class DebugMenu
{
public:
	DebugMenu();
	~DebugMenu();

	void Update(const float dt, GameTechRenderer* renderer);
	void Toggle() { debugMenuOpen = !debugMenuOpen; }
protected:
	float frameTime = 60.0f;
	int calculateNewFPS = 0;
	bool debugMenuOpen = false;
};

