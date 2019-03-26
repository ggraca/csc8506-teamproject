#pragma once

#include "../Common/NetworkBase.h"


struct InstantiatePacket : public GamePacket {
	int prefabId, networkId;
	Vec3 position;
	Quaternion rotation;

	InstantiatePacket(int pid, int nid, Vec3 pos, Quaternion rot) {
		type = BasicNetworkMessages::InstantiateMessage;
		size = sizeof(int) * 2 + sizeof(Vec3) + sizeof(Quaternion);

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
	Vec3 position;
	Quaternion rotation;

	ObjectUpdatePacket(int nid, Vec3 pos, Quaternion rot) {
		type = BasicNetworkMessages::ObjectUpdateMessage;
		size = sizeof(int) + sizeof(Vec3) + sizeof(Quaternion);

		networkId = nid;
		position = pos;
		rotation = rot;
	}
};

struct PlayerInputPacket : public GamePacket {
	short keysDown;
	short keysPressed;
	Vec3 cameraPosition;
	Quaternion cameraRotation;

	PlayerInputPacket(short kd, short kp, Vec3 cameraPos, Quaternion cameraRot) {
		type = BasicNetworkMessages::PlayerInputMessage;
		size = sizeof(short) * 2 + sizeof(Vec3) + sizeof(Quaternion);

		keysDown = kd;
		keysPressed = kp;
		cameraPosition = cameraPos;
		cameraRotation = cameraRot;
	}
};