#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "../GameTechCommon/PhysicsSystem.h"

#include "../GameTechCommon/GameServer.h"
#include "../GameTechCommon/GameClient.h"

#include "../GameTechCommon/NavigationGrid.h"
#include "Console.h"
#include "DebugMenu.h"

namespace NCL {
  namespace CSC8503 {
    class PhysicsScene : public Scene/*, public PacketReceiver*/ {
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