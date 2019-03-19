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

struct DestroyPacket : public GamePacket {
	int networkId;

	DestroyPacket(int nid) {
		type = BasicNetworkMessages::DestroyMessage;
		size = sizeof(int);

		networkId = nid;
	}
};

struct ObjectUpdatePacket : public GamePacket {
	int networkId;
	Vector3 position;
	Quaternion rotation;

	ObjectUpdatePacket(int nid, Vector3 pos, Quaternion rot) {
		type = BasicNetworkMessages::ObjectUpdateMessage;
		size = sizeof(int) + sizeof(Vector3) + sizeof(Quaternion);

		networkId = nid;
		position = pos;
		rotation = rot;
	}
};

struct PlayerInputPacket : public GamePacket {
	short keysDown;
	short keysPressed;

	PlayerInputPacket(short kd, short kp) {
		type = BasicNetworkMessages::PlayerInputMessage;
		size = sizeof(short) * 2;

		keysDown = kd;
		keysPressed = kp;
	}
};