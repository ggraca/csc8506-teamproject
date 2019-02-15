#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "../GameTechCommon/PhysicsSystem.h"

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

      DebugMenu debugMenu;
      Console console;
    };
  }
}