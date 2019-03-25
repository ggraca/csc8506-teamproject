#pragma once
#ifdef _WIN32

#include <thread>
#include <atomic>

#include "NetworkBase.h"

namespace NCL {
	namespace Networking {

		class GameServer : public NetworkBase {
		public:
			GameServer(int onPort, int maxClients);
			~GameServer();

			bool Initialise();
			void Shutdown();

			// void SetGameWorld(GameWorld &g);

			void ThreadedUpdate();

			bool SendGlobalPacket(int msgID);
			bool SendGlobalPacket(GamePacket& packet);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();

			virtual void UpdateServer();

		protected:
			int			port;
			int			clientMax;
			int			clientCount;

			std::atomic<bool> threadAlive;

			std::thread updateThread;

			int incomingDataRate;
			int outgoingDataRate;

			std::map<int, int> stateIDs;
		};
	}
}
#endif