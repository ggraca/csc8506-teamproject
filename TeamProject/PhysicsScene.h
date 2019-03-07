#pragma once

#include "GameTechRenderer.h"
#include "Scene.h"

#include "Console.h"
#include "DebugMenu.h"
#include "Player.h"
#include "Resource.h"
#include "CameraControl.h"

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
	  const btCollisionObject* Raycast(const Vector3& Start, Vector3& End, Vector3& NewEnd, Vector3& Normal);

	  GameObject* bestcube;

      DebugMenu debugMenu;
      Console console;
	  HUD hud;
    };
  }
}