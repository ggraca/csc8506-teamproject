#pragma once

#include "Scene.h"
#include "../Common/NetworkBase.h"
#include "../Common/GameServer.h"
#include "../Common/GameClient.h"

using namespace NCL::Networking;

namespace NCL {
	class NetworkScene : public Scene, public PacketReceiver {
	public:
		NetworkScene();
		virtual ~NetworkScene();
		void UpdateGame(float dt);

	protected:
		void InitNetwork();
		void ReceivePacket(int type, GamePacket* payload, int source);

		bool isServer = false;
		GameServer* server;
		GameClient* client;
	};
}