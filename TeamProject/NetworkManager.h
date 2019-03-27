#pragma once

#include <vector>

#include "NetworkEntity.h"
#include "NetworkClient.h"
#include "NetworkServer.h"


class NetworkManager {
public:
	NetworkManager(bool server);
	~NetworkManager();

	void Update();

	NetworkEntity* GetEntity() const {
		return networkEntity;
	}

private:
	bool CreateServer();
	void CreateClient();

	bool isServer;
	NetworkEntity* networkEntity;

	int port;
};