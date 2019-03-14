#pragma once

#include "../Common/NetworkBase.h"


struct InstantiatePacket : public GamePacket {
	int networkId, prefabId;
	float x, y, z;

	InstantiatePacket(int nid, int pid, float posx, float posy, float posz) {
		type = BasicNetworkMessages::InstantiateMessage;
		size = sizeof(int) * 2 + sizeof(float) * 3;

		networkId = nid;
		prefabId = pid;
		x = posx;
		y = posy;
		z = posz;
	}
};