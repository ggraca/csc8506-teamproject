#pragma once

#include "Scene.h"
#include "../Common/NetworkBase.h"

namespace NCL {
	class NetworkScene : public Scene, public PacketReceiver {
	public:
		NetworkScene();
		void UpdateGame(float dt);

	protected:
		void InitNetwork();
		void ResetWorld();
		void ReceivePacket(int type, GamePacket* payload, int source) {};
	};
}