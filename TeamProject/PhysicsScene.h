#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"
#include "Console.h"
#include "DebugMenu.h"
#include "Player.h"
#include "Resource.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "Destructible.h"


class PhysicsScene : public Scene {
public:
    PhysicsScene();
    ~PhysicsScene();
    void UpdateGame(float dt);
	
protected:
    void ResetWorld();
    void DebugScene(float dt);
	void UpdateKeys();

	GameObject* bestcube;

    DebugMenu debugMenu;
    Console console;
	HUD hud;
};