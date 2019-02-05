#pragma once
#include "GameTechRenderer.h"

class DebugMenu
{
public:
	DebugMenu();
	~DebugMenu();

	void Update(const float dt, const GameTechRenderer& renderer) const ;
protected:
	float frameTime = 60.0f;
	int calculateNewFPS = 0;
};

