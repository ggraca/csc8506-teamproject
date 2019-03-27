//#include "NetworkManager.h"
//
//
//NetworkManager::NetworkManager(bool server) {
//	NetworkBase::Initialise();
//	port = NetworkBase::GetDefaultPort();
//
//	if (server) CreateServer();
//	else CreateClient();
//}
//
//NetworkManager::~NetworkManager() {
//	NetworkBase::Destroy();
//}
//
//void NetworkManager::Update() {
//	networkEntity->Update();
//}
//
//bool NetworkManager::CreateServer()
//{
//	GameServer* server = new GameServer(port, 4);
//	isServer = server->Initialise();
//	
//	if (isServer)
//	{
//		networkEntity = new NetworkServer(server);
//		server->RegisterConnectionHandler(networkEntity);
//		server->RegisterPacketHandler(StringMessage, networkEntity);
//		server->RegisterPacketHandler(PlayerInputMessage, networkEntity);
//		std::cout << "Server created..." << std::endl;
//	}
//	else
//	{
//		std::cout << "Server already exists. Connecting to server..." << std::endl;
//	}
//
//	return isServer;
//}
//
//void NetworkManager::CreateClient()
//{
//	GameClient* client = new GameClient();
//	networkEntity = new NetworkClient(client);
//	client->RegisterPacketHandler(StringMessage, networkEntity);
//	client->RegisterPacketHandler(InstantiateMessage, networkEntity);
//	client->RegisterPacketHandler(DestroyMessage, networkEntity);
//	client->RegisterPacketHandler(ObjectUpdateMessage, networkEntity);
//
//	bool canConnect = client->Connect(127, 0, 0, 1, port);
//	if (canConnect) std::cout << "Conneced to Server" << std::endl;
//}