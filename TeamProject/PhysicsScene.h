#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"

#include "DebugMenu.h"
#include "Player.h"
#include "Resource.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "Destructible.h"

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

	  GameObject* bestcube;

      DebugMenu debugMenu;
	  HUD hud;
    };
  }
}