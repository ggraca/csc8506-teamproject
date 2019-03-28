#pragma once

#include "Component.h"


class NetworkObject : public Component {
public:
	enum prefabIds {
		Player,
		Resource,
		Cube,
		Cannon,
		Cart,
		Castle,
		DWall,
		Market,
		Stall,
		Tent,
		Tower,
		Wall,
		Well
	};

	NetworkObject(GameObject* go, int id) : Component(go), prefabId(id) {}
	
	void SetId(int i) { id = i; }
	int GetId() const { return id; }
	void SetHash(size_t h) { transformHash = h; }
	size_t GetHash() const { return transformHash; }
	int GetPrefabId() const { return prefabId; }

private:
	int id;
	int prefabId;
	size_t transformHash;
};