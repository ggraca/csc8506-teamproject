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

using namespace NCL::Networking;

class PhysicsScene : public GameScene {
public:
	PhysicsScene();
	~PhysicsScene();
    
	bool CreateServer();
	void CreateClient();

	void LateUpdate(float dt) override;

protected:
	void ResetWorld();
	void UpdateKeys();
	void InitializeGuns(GameObject * player);
	void InitializeShield(GameObject * player);
	void InitializeBigGun(GameObject * player);
	void InitPlayer();
	void InitializeHammer(GameObject * player);
    
	bool isServer = false;

	GameServer* server;
	vector<GameClient*> clients;
	GameObject* bestcube;
};
