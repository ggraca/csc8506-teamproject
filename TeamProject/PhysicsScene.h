#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"

#include "../Common/NetworkBase.h"
#include "../Common/GameServer.h"
#include "../Common/GameClient.h"

#include "Console.h"
#include "DebugMenu.h"
#include "Player.h"
#include "Resource.h"
#include "CameraControl.h"

namespace NCL {
  namespace CSC8503 {
	using namespace NCL::Networking;
    class PhysicsScene : public Scene {
    public:
      PhysicsScene();
      virtual ~PhysicsScene();
      void UpdateGame(float dt);

	  bool CreateServer();
	  void CreateClient();
	  void CreatePlayer();

    protected:
      void ResetWorld();
      void DebugScene(float dt);
	  void UpdateKeys();

	  GameObject* bestcube;

      DebugMenu debugMenu;
      Console console;
	  HUD hud;

	  bool isServer = false;

	  GameServer* server;
	  vector<GameClient*> clients;
    };
  }
}