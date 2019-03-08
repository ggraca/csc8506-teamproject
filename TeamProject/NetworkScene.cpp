#include <fstream>
#include <string>

#include "NetworkScene.h"
#include "GameWorld.h"
#include "../Common/TextureLoader.h"
#include "../Common/Assets.h"

using namespace NCL;

NetworkScene::NetworkScene() : Scene() {
	if (CreateServer());
	else CreateClient();
	//InitNetwork();
}

NetworkScene::~NetworkScene() {
	// Reset status
	if (isServer)
	{
		string filename = Assets::DATADIR + "networkstatus.txt";
		std::ofstream outfile(filename);
		outfile << 0;
		outfile.close();
	}
}

void NetworkScene::InitNetwork() {
	/*NetworkBase::Initialise();

	int serverRunning;
	int port = NetworkBase::GetDefaultPort();


	// Read status from file
	string filename = Assets::DATADIR + "networkstatus.txt";
	std::ifstream infile(filename);
	infile >> serverRunning;
	infile.close();

	if (!serverRunning) {
		isServer = true;
		std::ofstream outfile(filename);
		outfile << 1;
		outfile.close();
	}

	if (isServer) {
		server = new GameServer(port, 5);
		server->RegisterPacketHandler(StringMessage, this);
	}
	else {
		client = new GameClient();
		client->RegisterPacketHandler(StringMessage, this);
		bool canConnect = client->Connect(127, 0, 0, 1, port);
	}*/
}

bool NetworkScene::CreateServer()
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

void NetworkScene::CreateClient()
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

void NetworkScene::UpdateGame(float dt) {
	world->UpdateWorld(dt);

	renderer->Update(dt);
	physics->Update(dt);
	renderer->Render();

	//Send position with floats
	if (isServer) server->SendGlobalPacket(PlayerPosPacket(5.0f, 5.0f, 5.0f));
	else
	{
		clients[0]->SendPacket(PlayerPosPacket(1.0f, 1.0f, 1.0f));
		clients[0]->UpdateClient();
	}
	if (isServer) server->UpdateServer();


	
	//Messages
	/*if (isServer) server->SendGlobalPacket(StringPacket("Update Game on server run"));
	else
	{
		for (unsigned int i = 0; i < clients.size(); i++)
		{
			clients[i]->SendPacket(StringPacket("UpdateGame on client run"));
			clients[i]->UpdateClient();
		}
	}
	if (isServer) server->UpdateServer();*/
}

void NetworkScene::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == StringMessage)
	{
		StringPacket* realPacket = (StringPacket*) payload;
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