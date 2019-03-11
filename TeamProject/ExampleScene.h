#pragma once
#include "GameTechRenderer.h"
#include "GameScene.h"
#include "Console.h"
#include "DebugMenu.h"
#include "HUD.h"
#include "HUDObject.h"
#include "Animation.h"
#include "Player.h"
#include "Resource.h"
#include "CameraControl.h"


class ExampleScene : public GameScene {
public:
	ExampleScene();
	~ExampleScene() {};

protected:
	void ResetWorld();
};