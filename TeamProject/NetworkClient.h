#pragma once

#include "NetworkEntity.h"
#include "../Common/GameClient.h"

using namespace NCL::Networking;


class NetworkClient : public NetworkEntity {
public:
	NetworkClient(GameClient* gc) : client(gc) {}
	~NetworkClient() { delete client; }

	void Update() {
		client->UpdateClient();
		client->SendPacket(StringPacket("hello!"));
	}
	
	void Instantiate(GameObject* go) override {}
	void Destroy() override {};

private:
	void ReceivePacket(int type, GamePacket* payload, int source) override;

	GameClient* client;
};