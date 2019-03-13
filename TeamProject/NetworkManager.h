#pragma once

#include <vector>

#include "../Common/NetworkBase.h"
#include "../Common/GameServer.h"
#include "../Common/GameClient.h"


using namespace NCL::Networking;

class NetworkManager : public PacketReceiver {
public:
	NetworkManager();

private:
	bool CreateServer();
	void CreateClient();

	void ReceivePacket(int type, GamePacket* payload, int source);

	bool isServer;
	GameServer* server;
	vector<GameClient*> clients;
};