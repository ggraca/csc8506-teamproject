#include "NetworkClient.h"
#include "GameWorld.h"
#include "NetworkPackets.h"

#include "PlayerPrefab.h"
#include "ResourcePrefab.h"
#include "WallPrefab.h"
#include "StallPrefab.h"
#include "TentPrefab.h"
#include "MarketPrefab.h"
#include "WellPrefab.h"
#include "CastlePrefab.h"
#include "TowerPrefab.h"
#include "CannonPrefab.h"
#include "DWallPrefab.h"
#include "CartPrefab.h"


GameObject* NetworkClient::GetGameObjectFromPacket(InstantiatePacket* packet) {
	switch (packet->prefabId) {
	case NetworkObject::Resource:
		return new ResourcePrefab(packet->position, packet->rotation, Vector3(5, 5, 5), 1000, 0.2f, 0.4f);
	case NetworkObject::Player: {
		GameObject* player = new PlayerPrefab(packet->position, packet->rotation, Vector3(10, 10, 10), 100, 0.2f, 0.4f);
		// player->AddComponent<PlayerMovement*>(new PlayerMovement());
		world->GetMainCamera()->GetComponent<CameraControl*>()->SetPlayer(player);
		return player;
	}
	case NetworkObject::Cube:
		return new CubePrefab(packet->position, packet->rotation, packet->scale);
	case NetworkObject::Wall:
		return new WallPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::Stall:
		return new StallPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::Tent:
		return new TentPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::Market:
		return new MarketPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::Well:
		return new WellPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::Castle:
		return new CastlePrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::Tower:
		return new TowerPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::Cannon:
		return new CannonPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::DWall:
		return new DWallPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	case NetworkObject::Cart:
		return new CartPrefab(Vector3(1, 1, 1), packet->position, packet->rotation);
	}
	return nullptr;
}

void NetworkClient::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == StringMessage) {
		StringPacket* realPacket = (StringPacket*)payload;
		string msg = realPacket->GetStringFromData();
		std::cout << "received message: " << msg << std::endl;
	}
	else if (type == InstantiateMessage) {
		InstantiatePacket* packet = (InstantiatePacket*) payload;
		GameObject* go = GetGameObjectFromPacket(packet);
		go->RemoveComponent<PhysicsObject*>();
		world->Instantiate(go);

		NetworkObject* no = go->GetComponent<NetworkObject*>();
		no->SetId(packet->networkId);
		objects.push_back(no);
	}
	else if (type == DestroyMessage) {
		DestroyPacket* packet = (DestroyPacket*)payload;

		NetworkObject* no = FindObject(packet->networkId);
		if (!no) return;

		objects.erase(remove(objects.begin(), objects.end(), no), objects.end());
		world->LateDestroy(no->GetGameObject());
	}
	else if (type == ObjectUpdateMessage) {
		ObjectUpdatePacket* packet = (ObjectUpdatePacket*)payload;

		NetworkObject* no = FindObject(packet->networkId);
		if (!no) return;

		no->GetGameObject()->GetTransform().SetWorldPosition(packet->position);
		no->GetGameObject()->GetTransform().SetLocalOrientation(packet->rotation);
	}
}
