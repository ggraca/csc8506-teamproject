#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"
#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"
#include "GameScene.h"
#include "Player.h"
#include "Resource.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "Destructible.h"
#include "PauseMenu.h"

	  void InitStateMachine();
	  void ShowMenu();

class PhysicsScene : public GameScene {
public:
    PhysicsScene();
    ~PhysicsScene();

    void LateUpdate(float dt) override;
	
protected:
    void ResetWorld();
	void UpdateKeys();

	GameObject* bestcube;
};
	  bool showPauseMenu = false;
	  LevelState* worldState;
	  void UsedForMenu(void* data);
	  int currentMenuPath = 0;
	  bool& quitGame;
	  StateMachine* objectStateMachine;