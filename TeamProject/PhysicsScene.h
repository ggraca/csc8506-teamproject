#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"

#include "Console.h"
#include "DebugMenu.h"

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

      DebugMenu debugMenu;
      Console console;
    };
  }
}