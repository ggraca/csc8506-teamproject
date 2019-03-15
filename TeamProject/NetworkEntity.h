#pragma once

#include "../Common/NetworkBase.h"
#include "NetworkObject.h"

class NetworkEntity : public PacketReceiver {
public:
	virtual void Update() = 0;
	virtual void Instantiate(GameObject* go) = 0;
	virtual void Destroy() = 0;

	void SetWorld(GameWorld* w) { world = w; }

protected:
	NetworkObject* FindObject(int id) {
		for (auto o : objects) {
			if (o->GetId() == id) return o;
		}
		return nullptr;
	}

	GameWorld* world = nullptr;
	vector<NetworkObject*> objects;
};