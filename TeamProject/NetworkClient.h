#pragma once

#include "NetworkEntity.h"
#include "../Common/GameClient.h"
#include "NetworkPackets.h"
#include "InputManager.h"
#include "GameWorld.h"

using namespace NCL::Networking;


class NetworkClient : public NetworkEntity {
public:
	NetworkClient(GameClient* gc) : client(gc) {}
	~NetworkClient() { delete client; }

	void Update() {
		client->UpdateClient();
		client->SendPacket(PlayerInputPacket(
			InputManager::GetInputBitsDown(),
			InputManager::GetInputBitsPressed(),
			world->GetMainCamera()->GetTransform().GetWorldOrientation()
		));
	}
	
	void Instantiate(GameObject* go) override {}
	void Destroy() override {};

private:
	void ReceivePacket(int type, GamePacket* payload, int source) override;
	GameObject* GetGameObjectFromPacket(InstantiatePacket* packet);

	GameClient* client;
};