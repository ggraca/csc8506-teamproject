#pragma once

#include "Component.h"

class NetworkObject : public Component {
public:
	NetworkObject(GameObject* go) : Component(go) {}
	
	void SetId(int i) { id = i; }
	int GetId() const { return id; }
	void SetHash(size_t h) { transformHash = h; }
	size_t GetHash() const { return transformHash; }

private:
	int id;
	size_t transformHash;
};