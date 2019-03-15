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
		InstantiatePacket* packet = (InstantiatePacket*) payload;
		auto resource1 = new ResourcePrefab(packet->position, packet->rotation, Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
		resource1->RemoveComponent<PhysicsObject*>();
		world->Instantiate(resource1);

		NetworkObject* no = resource1->GetComponent<NetworkObject*>();
		no->SetId(packet->networkId);
		objects.push_back(no);
	}
	else if (type == ObjectUpdateMessage) {
		ObjectUpdatePacket* packet = (ObjectUpdatePacket*)payload;
		
		NetworkObject* no = FindObject(packet->networkId);
		if (!no) return;


		no->GetGameObject()->GetTransform().SetWorldPosition(packet->position);
		no->GetGameObject()->GetTransform().SetLocalOrientation(packet->rotation);
	}
}