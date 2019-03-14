#pragma once

#include "Component.h"

class NetworkObject : public Component {
public:
	NetworkObject(GameObject* go) : Component(go) {}
	void SetId(int i) { id = i; }
	int GetId() const { return id; }

private:
	int id;
	string transformHash;
};