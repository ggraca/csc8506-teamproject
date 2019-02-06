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
        class ExampleScene : public Scene/*, public PacketReceiver*/ {
        public:
            ExampleScene();
            ~ExampleScene();
            void UpdateGame(float dt);
        protected:
            void ResetWorld();
			void DebugScene(float dt);

			DebugMenu debugMenu;
			Console console;

            /*
            GameServer* server;
            GameClient* client;
            */
        };
    }
}
