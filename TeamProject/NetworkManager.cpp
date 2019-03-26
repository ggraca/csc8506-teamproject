#include "NetworkManager.h"


NetworkManager::NetworkManager(bool server) {
	NetworkBase::Initialise();
	port = NetworkBase::GetDefaultPort();

	if (server) CreateServer();
	else CreateClient();
}

NetworkManager::~NetworkManager() {
	NetworkBase::Destroy();
}

void NetworkManager::Update() {
	networkEntity->Update();
}

bool NetworkManager::CreateServer()
{
	GameServer* server = new GameServer(port, 4);
	isServer = server->Initialise();
	
	if (isServer)
	{
		networkEntity = new NetworkServer(server);
		server->RegisterConnectionHandler(networkEntity);
		server->RegisterPacketHandler(StringMessage, networkEntity);
		server->RegisterPacketHandler(PlayerInputMessage, networkEntity);
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
	GameClient* client = new GameClient();
	networkEntity = new NetworkClient(client);
	client->RegisterPacketHandler(StringMessage, networkEntity);
	client->RegisterPacketHandler(InstantiateMessage, networkEntity);
	client->RegisterPacketHandler(DestroyMessage, networkEntity);
	client->RegisterPacketHandler(ObjectUpdateMessage, networkEntity);
	client->RegisterPacketHandler(PlayerStateMessage, networkEntity);

	ifstream file;
	file.open("../Assets/Data/ip.txt");
	int connectingIP[4];

	if (file.is_open())
	{
		string line;
		int lineNumber = 0;
		while (getline(file, line))
		{
			connectingIP[lineNumber] = stoi(line);
			lineNumber += 1;
		}
	}
	file.close();


	bool canConnect = client->Connect(connectingIP[0], connectingIP[1], connectingIP[2], connectingIP[3], port);
	if (canConnect) std::cout << "Connected to Server" << std::endl;

}