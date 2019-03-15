#pragma once

#include "../Common/NetworkBase.h"


struct InstantiatePacket : public GamePacket {
	int prefabId, networkId;
	Vector3 position;
	Quaternion rotation;

	InstantiatePacket(int pid, int nid, Vector3 pos, Quaternion rot) {
		type = BasicNetworkMessages::InstantiateMessage;
		size = sizeof(int) * 2 + sizeof(Vector3) + sizeof(Quaternion);

		networkId = nid;
		prefabId = pid;
		position = pos;
		rotation = rot;
	}
};

struct ObjectUpdatePacket : public GamePacket {
	int networkId;
	Vector3 position;
	Quaternion rotation;

	ObjectUpdatePacket(int nid, Vector3 pos, Quaternion rot) {
		type = BasicNetworkMessages::ObjectUpdateMessage;
		size = sizeof(int) * 2 + sizeof(Vector3) * 3;

		networkId = nid;
		position = pos;
		rotation = rot;
	}
};