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
	GameWorld* world = nullptr;
	vector<NetworkObject*> objects;
};