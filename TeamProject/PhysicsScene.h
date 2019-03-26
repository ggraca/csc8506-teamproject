#pragma once

#include "GameTechRenderer.h"
#include "GameScene.h"
#include "Player.h"
#include "Resource.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "Destructible.h"
#include "../Common/NetworkBase.h"
#include "../Common/GameServer.h"
#include "../Common/GameClient.h"
#ifdef _WIN32
using namespace NCL::Networking;
#endif

class PhysicsScene : public GameScene {
public:
	PhysicsScene(bool& quitGame);
	~PhysicsScene() {};
#ifdef _WIN32  
	bool CreateServer();
	void CreateClient();
#endif
	void LateUpdate(float dt) override;

protected:
	void ResetWorld();
	void UpdateKeys();
  
	bool isServer = false;
#ifdef _WIN32
	GameServer* server;
	vector<GameClient*> clients;
	GameObject* bestcube;
#endif
	
};
