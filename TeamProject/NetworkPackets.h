#pragma once

#include "../Common/NetworkBase.h"


struct InstantiatePacket : public GamePacket {
	int prefabId, networkId;
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	bool isActive;

	InstantiatePacket(int pid, int nid, Vector3 pos, Quaternion rot, Vector3 sca, bool ia = true) {
		type = BasicNetworkMessages::InstantiateMessage;
		size = sizeof(int) * 2 + sizeof(Vector3) * 2 + sizeof(Quaternion) + sizeof(bool);

		networkId = nid;
		prefabId = pid;
		position = pos;
		rotation = rot;
		scale = sca;
		isActive = ia;
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
	Vector3 scale;
	Vector3 colour;
	bool isActive;

	ObjectUpdatePacket(int nid, Vector3 pos, Quaternion rot, Vector3 sca, Vector3 col, bool ia = true) {
		type = BasicNetworkMessages::ObjectUpdateMessage;
		size = sizeof(int) + sizeof(Vector3) * 3 + sizeof(Quaternion) + sizeof(bool);

		networkId = nid;
		position = pos;
		rotation = rot;
		scale = sca;
		colour = col;
		isActive = ia;
	}
};

struct PlayerInputPacket : public GamePacket {
	short keysDown;
	short keysPressed;
	Vector3 cameraPosition;
	Quaternion cameraRotation;

	PlayerInputPacket(short kd, short kp, Vector3 cameraPos, Quaternion cameraRot) {
		type = BasicNetworkMessages::PlayerInputMessage;
		size = sizeof(short) * 2 + sizeof(Vector3) + sizeof(Quaternion);

		keysDown = kd;
		keysPressed = kp;
		cameraPosition = cameraPos;
		cameraRotation = cameraRot;
	}
};

struct PlayerStatePacket : public GamePacket {
	PlayerState playerState;

	PlayerStatePacket(PlayerState ps) : playerState(ps) {
		type = BasicNetworkMessages::PlayerStateMessage;
		size = sizeof(PlayerState);
	}
};