#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "../GameTechCommon/PhysicsSystem.h"
#include "../GameTechCommon/GameServer.h"
#include "../GameTechCommon/GameClient.h"
#include "../GameTechCommon/NavigationGrid.h"
#include "Console.h"
#include "DebugMenu.h"
#include "HUD.h"
#include "InputManager.h"



namespace NCL {
    namespace CSC8503 {
<<<<<<< HEAD:TeamProject/PhysicsScene.h
        class PhysicsScene : public Scene/*, public PacketReceiver*/ {
=======
		
        class ExampleScene : public Scene/*, public PacketReceiver*/ {
>>>>>>> master:TeamProject/ExampleScene.h
        public:
            PhysicsScene();
            ~PhysicsScene();
            void UpdateGame(float dt);
			InputManager* GetInputManager() const;
        protected:
            void ResetWorld();
			void RegisterConsoleCommands();

			DebugMenu debugMenu;
			HUD hud;
			Console console;
			InputManager * inputManager;

            /*
            GameServer* server;
            GameClient* client;
            */
        };
    }
}
