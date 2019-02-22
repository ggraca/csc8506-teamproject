#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "Console.h"
#include "DebugMenu.h"
#include "HUD.h"
#include "HUDObject.h"
#include "Animation.h"
#include "InputManager.h"


namespace NCL {
  namespace CSC8503 {

    class ExampleScene : public Scene {
    public:
		ExampleScene();
		~ExampleScene();
		void UpdateGame(float dt);
		InputManager* GetInputManager() const;

    protected:
		void ResetWorld();
		void RegisterConsoleCommands();

		DebugMenu debugMenu;
		vector<HUDObject> hudElements;
		HUD hud;
		Console console;
		InputManager * inputManager;
    };
  }
}
