#include "NetworkManager.h"


NetworkManager::NetworkManager() {
	if (CreateServer());
	else CreateClient();
}

bool NetworkManager::CreateServer()
{
	NetworkBase::Initialise();

	TestPacketReceiver* serverReceiver = new TestPacketReceiver("Server");

	int port = NetworkBase::GetDefaultPort();

	server = new GameServer(port, 4);

	if (server->Initialise())
	{
		std::cout << "Creating server..." << std::endl;
		isServer = true;
		server->RegisterPacketHandler(StringMessage, serverReceiver);
		server->RegisterPacketHandler(PlayerPos, serverReceiver);
		std::cout << "Server created..." << std::endl;
		return isServer;
	}
	else
	{
		std::cout << "Server already exists. Connecting to server..." << std::endl;
		return false;
	}
}

void NetworkManager::CreateClient()
{
	TestPacketReceiver* clientReceiver = new TestPacketReceiver("Client");
	int port = NetworkBase::GetDefaultPort();
	GameClient* client = new GameClient();
	client->RegisterPacketHandler(StringMessage, clientReceiver);
	client->RegisterPacketHandler(PlayerPos, clientReceiver);
	bool canConnect = client->Connect(127, 0, 0, 1, port);
	if (canConnect) std::cout << "New client connected!" << std::endl;
	clients.emplace_back(client);
}

void NetworkManager::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == StringMessage)
	{
		StringPacket* realPacket = (StringPacket*)payload;
		string msg = realPacket->GetStringFromData();
		std::cout << "received message: " << msg << std::endl;
	}
	else if (type == PlayerPos)
	{
		PlayerPosPacket* realPacket = (PlayerPosPacket*)payload;
		float* pos = realPacket->GetPosFromData();
		std::cout << "X: " << pos[0] << " Y: " << pos[1] << " Z: " << pos[2] << std::endl;
	}

}