#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "../GameTechCommon/PhysicsSystem.h"

#include "../GameTechCommon/GameServer.h"
#include "../GameTechCommon/GameClient.h"

#include "../GameTechCommon/NavigationGrid.h"

enum BotState {
    Following,
    Returning,
    Patrolling,
    Dead // :(
};

namespace NCL {
    namespace CSC8503 {
        class ExampleScene : public Scene/*, public PacketReceiver*/ {
        public:
            ExampleScene();
            ~ExampleScene();
            void UpdateGame(float dt);
        protected:
            void ResetWorld();

            /*
            GameServer* server;
            GameClient* client;
            */
        };
    }
}
