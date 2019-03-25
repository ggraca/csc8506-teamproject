#pragma once

#include "../Common/NetworkBase.h"
#include "NetworkObject.h"


struct PlayerState {
	PlayerState() {}
	PlayerState(int hp, int res) {
		health = hp;
		resources = res;
	}
	
	int health;
	int resources;
};

class NetworkEntity : public PacketReceiver {
public:
	virtual void Update() = 0;
	virtual void Instantiate(GameObject* go) = 0;
	virtual void Destroy() = 0;

	void SetWorld(GameWorld* w) { world = w; }
	PlayerState GetPlayerState() const { return playerState; }

protected:
	NetworkObject* FindObject(int id) {
		for (auto o : objects) {
			if (o->GetId() == id) return o;
		}
		return nullptr;
	}

	GameWorld* world = nullptr;
	vector<NetworkObject*> objects;
	PlayerState playerState;
};