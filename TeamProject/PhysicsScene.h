#pragma once

#include "GameTechRenderer.h"
#include "GameScene.h"
#include "Player.h"
#include "Resource.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "Destructible.h"


class PhysicsScene : public GameScene {
public:
    PhysicsScene();
    ~PhysicsScene();

    void EarlyUpdate(float dt) override;
	
protected:
    void ResetWorld();
	void UpdateKeys();

	GameObject* bestcube;
};