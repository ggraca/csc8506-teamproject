#include "GameServer.h"
// #include "GameWorld.h"
#include <iostream>
#include <fstream>

using namespace NCL;
using namespace NCL::Networking;

GameServer::GameServer(int onPort, int maxClients)	{
	port		= onPort;
	clientMax	= maxClients;
	clientCount = 0;
	netHandle	= nullptr;
	threadAlive = false;

	//Initialise();
}

GameServer::~GameServer()	{
	Shutdown();
}

void GameServer::Shutdown() {
	SendGlobalPacket(BasicNetworkMessages::Shutdown);

	threadAlive = false;
	updateThread.join();

	enet_host_destroy(netHandle);
	netHandle = nullptr;
}

bool GameServer::Initialise()
{

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

	ENetAddress address;
	address.host = (connectingIP[3] << 24) | (connectingIP[2] << 16) | (connectingIP[1] << 8) | connectingIP[0];
	address.port = port;
	netHandle = enet_host_create(&address, clientMax, 1, 0, 0);
	

	if (!netHandle) {
		std::cout << __FUNCTION__
				  << " failed to create network handle!" << std::endl;
		return false;
	}
	return true;
}

bool GameServer::SendGlobalPacket(int msgID) {
	GamePacket packet;
	packet.type = msgID;
	return SendGlobalPacket(packet);
}

bool GameServer::SendGlobalPacket(GamePacket& packet) {
	ENetPacket* dataPacket = enet_packet_create(
		&packet, packet.GetTotalSize(), 0
	);
	enet_host_broadcast(netHandle, 0, dataPacket);
	return true;
}

bool GameServer::SendPacket(GamePacket& packet, int peerId) {
	ENetPacket* dataPacket = enet_packet_create(
		&packet, packet.GetTotalSize(), 0
	);

	enet_peer_send(netHandle->peers + peerId, 0, dataPacket);
	return true;
}

void GameServer::UpdateServer() {
	if (!netHandle) return;

	ENetEvent event;
	while(enet_host_service(netHandle, &event, 0) > 0) {
		int type = event.type;
		ENetPeer* p = event.peer;
		int peer = p->incomingPeerID;

		if (type == ENetEventType::ENET_EVENT_TYPE_CONNECT) {
			if (connectionHandler) connectionHandler->OnClientConnect(peer);
		} else if (type == ENetEventType::ENET_EVENT_TYPE_DISCONNECT) {
			if (connectionHandler) connectionHandler->OnClientDisconnect(peer);
		} else if (type == ENetEventType::ENET_EVENT_TYPE_RECEIVE) {
			GamePacket* packet = (GamePacket*)event.packet->data;
			ProcessPacket(packet, peer);
		}
		enet_packet_destroy(event.packet);
	}
}

void GameServer::ThreadedUpdate() {
	while (threadAlive) {
		UpdateServer();
		std::this_thread::yield();
	}
}

void GameServer::BroadcastSnapshot(bool deltaFrame) {

}

void GameServer::UpdateMinimumState() {

}
