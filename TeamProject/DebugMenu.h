#pragma once
#include "GameTechRenderer.h"
#include "Debug.h"
#include "FunctionTimerManager.h"

using namespace NCL;
using namespace NCL::CSC8503;

class DebugMenu
{
public:
	DebugMenu(DebugMenu const&) = delete;
	void operator=(DebugMenu const&) = delete;

	static DebugMenu& GetInstance()
	{
		static DebugMenu instance;
		return instance;
	}

	static void Update(const float dt, GameTechRenderer* renderer);
	static void Toggle() { GetInstance().debugMenuOpen = !GetInstance().debugMenuOpen; }

protected:
	float frameTime = 60.0f;
	int calculateNewFPS = 0;
	bool debugMenuOpen = false;

	FunctionTimerManager timerManager;
private:
	DebugMenu() {};
};

