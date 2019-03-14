#pragma once

#include <vector>

#include "NetworkEntity.h"
#include "NetworkClient.h"
#include "NetworkServer.h"


class NetworkManager {
public:
	NetworkManager();
	~NetworkManager();

	void Update();

private:
	bool CreateServer();
	void CreateClient();

	bool isServer;
	NetworkEntity* networkEntity;

	int port;
};