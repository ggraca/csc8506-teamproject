#include <string>

#include "../Common/Utils.h"
#include "NetworkServer.h"
#include "NetworkPackets.h"
#include "PlayerPrefab.h"


void NetworkServer::Update() {
	server->UpdateServer();

	for (auto o : objects) {
		Transform t = o->GetGameObject()->GetTransform();
		Vector3 pos = t.GetWorldPosition();
		Quaternion rot = t.GetLocalOrientation();

		size_t hashed = std::hash<std::string>{}(ToString(pos) + ToString(rot));
		if (hashed == o->GetHash()) continue;

		o->SetHash(hashed);
		ObjectUpdatePacket p = ObjectUpdatePacket(o->GetId(), pos, rot);
		server->SendGlobalPacket(p);
	}

	PlayerState* ps = FindPlayer(0);
	if (ps) {
		ps->keysDown->UpdateInputs(InputManager::GetInputBitsDown());
		ps->keysPressed->UpdateInputs(InputManager::GetInputBitsPressed());
	}
	else {
		for (auto go : world->GetGameObjectList()) {
			if (go->GetComponent<Player*>() && go->GetComponent<NetworkObject*>()) {
				AddPlayer(0, go);
				return;
			}
		}
	}
}

void NetworkServer::OnClientConnect(int source) {
	for (auto o : objects) {
		Transform t = o->GetGameObject()->GetTransform();
		Vector3 pos = t.GetWorldPosition();
		Quaternion rot = t.GetLocalOrientation();

		InstantiatePacket p = InstantiatePacket(o->GetPrefabId(), o->GetId(), pos, rot);
		server->SendGlobalPacket(p);
	}

	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	world->LateInstantiate(player);

	auto shield = new CubePrefab(CubePrefab::PrefabType::SHIELD);
	GameObject * shieldDummy = new GameObject();
	shieldDummy->SetParent(player);
	shieldDummy->GetTransform().SetLocalPosition(Vector3(0, 2.5f, 5));

	player->GetComponent<ShieldControl*>()->SetShield(shield);
	player->GetComponent<ShieldControl*>()->SetTarget(&shieldDummy->GetTransform());
	player->GetComponent<ShieldControl*>()->SetShieldDummy(shieldDummy);

	world->Instantiate(shieldDummy);
	world->Instantiate(shield);

	AddPlayer(source, player);
}

void NetworkServer::OnClientDisconnect(int source) {
	RemovePlayer(source);
}

void NetworkServer::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == StringMessage)
	{
		StringPacket* realPacket = (StringPacket*)payload;
		string msg = realPacket->GetStringFromData();
		std::cout << "received message: " << msg << std::endl;
	}
	else if (type == PlayerInputMessage)
	{
		PlayerInputPacket* realPacket = (PlayerInputPacket*)payload;

		PlayerState* ps = FindPlayer(source);
		ps->keysDown->UpdateInputs(realPacket->keysDown);
		ps->keysPressed->UpdateInputs(realPacket->keysPressed);
	}
}

void NetworkServer::Instantiate(GameObject* go) {
	if (!go) return;

	NetworkObject* no = go->GetComponent<NetworkObject*>();
	if (!no) return;

	no->SetId(lastestId++);
	objects.push_back(no);

	server->SendGlobalPacket(InstantiatePacket(no->GetPrefabId(), no->GetId(), go->GetTransform().GetWorldPosition(), go->GetTransform().GetLocalOrientation()));
}

void NetworkServer::AddPlayer(int peerId, GameObject* go) {
	players.push_back(new PlayerState(peerId, go));
}

void NetworkServer::RemovePlayer(int peerId) {
	PlayerState* ps = FindPlayer(peerId);
	if (!ps) return;

	world->LateDestroy(ps->gameObject);
	objects.erase(remove(objects.begin(), objects.end(), ps->gameObject->GetComponent<NetworkObject*>()), objects.end());
	players.erase(remove(players.begin(), players.end(), ps), players.end());
}

PlayerState* NetworkServer::FindPlayer(int peerId) {
	for (auto p : players) {
		if (p->peerId == peerId) return p;
	}
	return nullptr;
}

PlayerState* NetworkServer::FindPlayer(GameObject* go) {
	for (auto p : players) {
		if (p->gameObject == go) return p;
	}
	return nullptr;
}