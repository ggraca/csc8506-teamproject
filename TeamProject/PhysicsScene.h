#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"
#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"

#include "Console.h"
#include "DebugMenu.h"
#include "Player.h"
#include "Resource.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "Destructible.h"
#include "Menu.h"

namespace NCL {
  namespace CSC8503 {
    class PhysicsScene : public Scene {
    public:
      PhysicsScene();
      ~PhysicsScene();
      void UpdateGame(float dt);
	
    protected:
      void ResetWorld();
      void DebugScene(float dt);
	  void UpdateKeys();
	  void InitStateMachine();
	  void ShowMenu();

	  bool showMenu = false;
	  LevelState* worldState;
	  void UsedForMenu(void* data);
	  int currentMenuPath = 0;

	  GameObject* bestcube;
      DebugMenu debugMenu;
      Console console;
	  HUD hud;
	  Menu menu;
	  
	  StateMachine* objectStateMachine;
    };
  }
}