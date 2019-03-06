#include <fstream>
#include <string>

#include "NetworkScene.h"
#include "GameWorld.h"
#include "../Common/TextureLoader.h"
#include "../Common/Assets.h"

using namespace NCL;

NetworkScene::NetworkScene() : Scene() {
	InitNetwork();
}

NetworkScene::~NetworkScene() {
	// Reset status
	string filename = Assets::DATADIR + "networkstatus.txt";
	std::ofstream outfile(filename);
	outfile << 0;
	outfile.close();
}

void NetworkScene::InitNetwork() {
	NetworkBase::Initialise();

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
	}
}

void NetworkScene::UpdateGame(float dt) {
	world->UpdateWorld(dt);

	renderer->Update(dt);
	physics->Update(dt);
	renderer->Render();
}

void NetworkScene::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type != StringMessage) return;

	StringPacket* realPacket = (StringPacket*) payload;
	string msg = realPacket->GetStringFromData();
	std::cout << "received message: " << msg << std::endl;
}