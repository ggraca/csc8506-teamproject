#pragma once

#include "NetworkEntity.h"
#include "../Common/GameClient.h"

using namespace NCL::Networking;


class NetworkClient : public NetworkEntity {
public:
	NetworkClient(GameClient* gc) : client(gc) {}
	~NetworkClient() { delete client; }

	void Update() { client->UpdateClient(); }

private:
	void ReceivePacket(int type, GamePacket* payload, int source);

	GameClient* client;
};