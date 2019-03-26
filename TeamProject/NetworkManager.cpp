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
	
	client->Connect(127, 0, 0, 1, port);
	return;

	ifstream file;
	
	int address[20];

	bool canConnect = false;

	file.open(Assets::DATADIR + "ip.txt");

	if (file.is_open())
	{
		string line;
		int lineNumber = 0;
		while (getline(file, line))
		{
			address[lineNumber] = stoi(line);		
			lineNumber += 1;
		}
		
	}

	file.close();

	for (int i = 0; i < 20; i += 4)
	{
		canConnect = client->Connect(address[i], address[i+1], address[i+2], address[i+3], port);
		if (canConnect)
		{
			std::cout << "Conneced to: " << "Address:  " << address[i] << ", " << address[i + 1] << ", " << address[i + 2] << ", " << address[i + 3] << ", " << port << " succeeded. " << std::endl;
			break;
		}
		else
		{
			std::cout << "Address:  " << address[i] << ", " << address[i + 1] << ", " << address[i + 2] << ", " << address[i + 3] << ", " << port << " failed. " << std::endl;
		}
	}
}