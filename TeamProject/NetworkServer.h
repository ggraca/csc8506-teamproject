#pragma once

#include "NetworkEntity.h"
#include "../Common/GameServer.h"

using namespace NCL::Networking;


class NetworkServer : public NetworkEntity {
public:
	NetworkServer(GameServer* gs) : server(gs) {}
	~NetworkServer() { delete server; }

	void Update() { server->UpdateServer(); }
	void Instantiate(GameObject* go) override;
	void Destroy() override {};

private:
	void OnClientConnect(int source) override;
	void OnClientDisconnect(int source) override;
	void ReceivePacket(int type, GamePacket* payload, int source) override;

	GameServer* server;
	int lastestId = 0;
};