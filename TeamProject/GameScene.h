#pragma once

#include "Scene.h"
#include "Console.h"
#include "DebugMenu.h"
#include "HUD.h"


class GameScene : public Scene {
public:
	GameScene();
	virtual ~GameScene() {};

protected:
	void LateUpdate(float dt) override;
	void RegisterConsoleCommands();

	DebugMenu debugMenu;
	Console console;
	HUD hud;
};