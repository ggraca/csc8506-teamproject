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

			//static void CreateTower()
			//{
			//	OBJMesh* m = new OBJMesh();
			//	m->LoadOBJMesh(MESHDIR"tinker.obj");
			//	tower = m;
			//}
			//static void DeleteTower() { delete tower; }

        protected:
            void ResetWorld();
			void DebugScene(float dt);

			DebugMenu debugMenu;
			Console console;

			void GenerateWorld();

			//static Mesh* tower;

            /*
            GameServer* server;
            GameClient* client;
            */
        };
    }
}
