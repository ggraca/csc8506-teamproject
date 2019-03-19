#pragma once

#include "NetworkEntity.h"
#include "../Common/GameServer.h"

using namespace NCL::Networking;

struct PlayerState {
	int peerId;
	GameObject* gameObject;

	PlayerState(int pi, GameObject* go) : peerId(pi), gameObject(go) {}
};

class NetworkServer : public NetworkEntity {
public:
	NetworkServer(GameServer* gs) : server(gs) {}
	~NetworkServer() { delete server; }

	void Update();
	void Instantiate(GameObject* go) override;
	void Destroy() override {};

	void AddPlayer(int peerId, GameObject* go) {
		players.push_back(new PlayerState(peerId, go));
	}

	void RemovePlayer(int peerId) {
		PlayerState* ps = FindPlayer(peerId);
		if (!ps) return;

		world->LateDestroy(ps->gameObject);
		objects.erase(remove(objects.begin(), objects.end(), ps->gameObject), objects.end());
		players.erase(remove(players.begin(), players.end(), ps), players.end());
	}

	PlayerState* FindPlayer(int peerId) {
		for (auto p : players) {
			if (p->peerId == peerId) return p;
		}
		return nullptr;
	}

private:
	void OnClientConnect(int source) override;
	void OnClientDisconnect(int source) override;
	void ReceivePacket(int type, GamePacket* payload, int source) override;

	GameServer* server;
	int lastestId = 0;
	vector<PlayerState*> players;
};