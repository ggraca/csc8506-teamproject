#pragma once

#include "../Common/NetworkBase.h"

class NetworkEntity : public PacketReceiver {
public:
	virtual void Update() = 0;
	virtual void Instantiate() {};
	virtual void Destroy() {};
};