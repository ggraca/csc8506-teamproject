#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "Console.h"
#include "DebugMenu.h"
#include "HUD.h"
#include "Animation.h"
#include "InputManager.h"
#include "Player.h"
#include "Resource.h"
#include "CameraControl.h"




namespace NCL {
  namespace CSC8503 {

    class ExampleScene : public Scene {
    public:
      ExampleScene();
      ~ExampleScene();
      void UpdateGame(float dt);
	  static InputManager * inputManager;

    protected:
      void ResetWorld();
	  void RegisterConsoleCommands();

	  DebugMenu debugMenu;
	  HUD hud;
	  Console console;
    };
  }
}
