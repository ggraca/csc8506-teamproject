#pragma once
#include "GameObject.h"
#include "Component.h"

namespace NCL {
	namespace CSC8503 {
	/*	struct FullPacket : public GamePacket {
			int		objectID;
			NetworkState fullState;

			FullPacket() {
				size = sizeof(FullPacket) + sizeof(int);
			}
		};*/

		/*struct DeltaPacket : public GamePacket {
			int		fullID;
			int		objectID;
			char	pos[3];
			char	orientation[4];

			DeltaPacket() {
				size = sizeof(DeltaPacket);
			}
		};

		struct ClientPacket : public GamePacket {
			int		lastID;
			char	buttonstates[8];

			ClientPacket() {
				size = sizeof(ClientPacket);
			}
		};*/

		class NetworkObject	: virtual public Component	{
		public:
			NetworkObject(GameObject* o, int id);
			virtual ~NetworkObject();

			//Called by clients
			virtual bool ReadPacket(/*GamePacket& p*/);
			//Called by servers
			virtual bool WritePacket(/*GamePacket** p*/ bool deltaFrame);

		protected:

			//virtual bool ReadDeltaPacket(/*DeltaPacket &p*/);
			//virtual bool ReadFullPacket(/*FullPacket &p*/);

			//virtual bool WriteDeltaPacket(/*GamePacket**p*/);
			//virtual bool WriteFullPacket(/*GamePacket**p*/);
		
			int networkID;
		};
	}
}

