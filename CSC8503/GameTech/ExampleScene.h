#pragma once
#include "GameTechRenderer.h"
#include "Scene.h"
#include "../CSC8503Common/PhysicsSystem.h"

#include "../CSC8503Common/GameServer.h"
#include "../CSC8503Common/GameClient.h"

#include "../CSC8503Common/NavigationGrid.h"

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
