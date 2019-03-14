#include "NetworkClient.h"
#include "GameWorld.h"
#include "ResourcePrefab.h"
#include "NetworkPackets.h"

void NetworkClient::ReceivePacket(int type, GamePacket* payload, int source) {
	;
	if (type == StringMessage) {
		StringPacket* realPacket = (StringPacket*)payload;
		string msg = realPacket->GetStringFromData();
		std::cout << "received message: " << msg << std::endl;
	}
	else if (type == InstantiateMessage) {
		InstantiatePacket* ip = (InstantiatePacket*) payload;
		auto resource1 = new ResourcePrefab(Vector3(ip->x, ip->y, ip->z), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
		resource1->RemoveComponent<PhysicsObject*>();
		resource1->GetComponent<NetworkObject*>()->SetId(ip->networkId);
		world->Instantiate(resource1);
	}
}