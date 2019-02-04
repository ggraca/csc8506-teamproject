#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "../GameTechCommon/PhysicsSystem.h"

#include "../GameTechCommon/GameServer.h"
#include "../GameTechCommon/GameClient.h"

#include "../GameTechCommon/NavigationGrid.h"

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

            /*
            GameServer* server;
            GameClient* client;
            */

			float frameTime = 60.0f;
			int calculateNewFPS = 0;
        };
    }
}
