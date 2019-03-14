#include "NetworkManager.h"


NetworkManager::NetworkManager() {
	NetworkBase::Initialise();
	port = NetworkBase::GetDefaultPort();

	if (CreateServer());
	else CreateClient();
}

NetworkManager::~NetworkManager() {
	NetworkBase::Destroy();
}

void NetworkManager::Update() {
	if (isServer) server->UpdateServer();
	else {
		client->UpdateClient();
		client->SendPacket(StringPacket("Hello!"));
	}
}

bool NetworkManager::CreateServer()
{
	server = new GameServer(port, 4);
	isServer = server->Initialise();
	
	if (isServer)
	{
		server->RegisterPacketHandler(StringMessage, this);
		server->RegisterPacketHandler(PlayerPos, this);
		std::cout << "Server created..." << std::endl;
	}
	else
	{
		std::cout << "Server already exists. Connecting to server..." << std::endl;
	}

	return isServer;
}

void NetworkManager::CreateClient()
{
	client = new GameClient();
	bool canConnect = client->Connect(127, 0, 0, 1, port);
	if (canConnect) std::cout << "Conneced to Server" << std::endl;

	client->RegisterPacketHandler(StringMessage, this);
	client->RegisterPacketHandler(PlayerPos, this);

	//client->SendPacket(StringPacket("Hello!"));
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