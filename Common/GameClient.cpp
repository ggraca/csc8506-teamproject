#include "GameClient.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace NCL;
using namespace NCL::Networking;

GameClient::GameClient()	{
	netHandle = enet_host_create(nullptr, 1, 1, 0, 0);


	/*ifstream file;
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

	ENetAddress address2;
	address2.host = (connectingIP[3] << 24) | (connectingIP[2] << 16) | (connectingIP[1] << 8) | connectingIP[0];
	address2.port = 1234;
	
	netHandle->address = address2;*/
}

GameClient::~GameClient()	{
	threadAlive = false;
	updateThread.join();
	enet_host_destroy(netHandle);
}

bool GameClient::Connect(uint8_t a, uint8_t b, uint8_t c, uint8_t d, int portNum) {
	ENetAddress* address = new ENetAddress();
	address->port = portNum;
	address->host = (d << 24) | (c << 16) | (b << 8) | a;

	netPeer = enet_host_connect(netHandle, address, 2, 0);
	//enet_address_set_host(address, "server");
	return netPeer != nullptr;
}

void GameClient::UpdateClient() {
	if (netHandle == nullptr) return;

	ENetEvent event;
	while(enet_host_service(netHandle, &event, 0) > 0) {
		if (event.type == ENET_EVENT_TYPE_CONNECT) {
			std::cout << "Connected to server!" << std::endl;
		} else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
			//std::cout << "Client: Packet received..." << std::endl;
			GamePacket* packet = (GamePacket*)event.packet->data;
			ProcessPacket(packet);
		}
		enet_packet_destroy(event.packet);
	}
}

void GameClient::SendPacket(GamePacket&  payload) {
	ENetPacket* dataPacket = enet_packet_create(
		&payload, payload.GetTotalSize(), 0
	);

	enet_peer_send(netPeer, 0, dataPacket);
}

void GameClient::ThreadedUpdate() {
	while (threadAlive) {
		UpdateClient();
		std::this_thread::yield();
	}
}
