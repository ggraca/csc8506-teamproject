#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"
#include "Console.h"
#include "DebugMenu.h"
#include "HUD.h"
#include "HUDObject.h"
#include "Animation.h"
#include "Player.h"
#include "Resource.h"
#include "CameraControl.h"


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