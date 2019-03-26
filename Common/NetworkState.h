#pragma once
#include "../Common/Vector3.h"
#include "../Common/Quaternion.h"

namespace NCL {
	using namespace Maths;

	namespace Networking {
		class GameObject;
		class NetworkState	{
		public:
			NetworkState() {
				stateID = 0;
			}
			virtual ~NetworkState();

			Vec3		position;
			Quaternion	orientation;
			int			stateID;
		};
	}
}

