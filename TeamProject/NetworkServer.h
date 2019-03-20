#pragma once

#include "NetworkEntity.h"
#include "../Common/GameServer.h"
#include "InputManager.h"

using namespace NCL::Networking;

struct PlayerState {
	PlayerState(int pi, GameObject* go) : peerId(pi), gameObject(go) {
		keysPressed = InputContainer();
		keysDown = InputContainer();
	}

	int peerId;
	GameObject* gameObject;
	InputContainer keysPressed;
	InputContainer keysDown;
	Quaternion cameraRotation;
};

class NetworkServer : public NetworkEntity {
public:
	NetworkServer(GameServer* gs) : server(gs) {}
	~NetworkServer() { delete server; }

	void Update();
	void Instantiate(GameObject* go) override;
	void Destroy() override {};

	void AddPlayer(int peerId, GameObject* go);
	void RemovePlayer(int peerId);
	PlayerState* FindPlayer(int peerId);
	PlayerState* FindPlayer(GameObject* go);

private:
	void OnClientConnect(int source) override;
	void OnClientDisconnect(int source) override;
	void ReceivePacket(int type, GamePacket* payload, int source) override;

	GameServer* server;
	int lastestId = 0;
	vector<PlayerState*> players;
};