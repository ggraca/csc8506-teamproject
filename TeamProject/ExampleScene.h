#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "DebugMenu.h"
#include "HUD.h"
#include "HUDObject.h"
#include "Animation.h"
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
	  void LoadWorld();

    protected:
      void ResetWorld();

      DebugMenu debugMenu;
      vector<HUDObject> hudElements;
      HUD hud;
      Console console;
    };
  }
}
