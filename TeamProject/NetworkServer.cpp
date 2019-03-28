#include <string>

#include "../Common/Utils.h"
#include "NetworkServer.h"
#include "NetworkPackets.h"
#include "PlayerPrefab.h"
#include "AudioEngine.h"


void NetworkServer::Update() {
	PlayerInput* ps = FindPlayer(-1);
	if (ps) {
		ps->keysDown = InputContainer(InputManager::GetInputBitsDown());
		ps->keysPressed = InputContainer(InputManager::GetInputBitsPressed());
		ps->cameraPosition = world->GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition();
		ps->cameraRotation = world->GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldOrientation();
	}
	else {
		for (auto go : world->GetGameObjectList()) {
			if (go->CompareTag(LayerAndTag::Tags::Player)) {
				AddPlayer(-1, go);
				world->GetAudio()->SetCamera(world->GetMainCamera());
				world->GetAudio()->SetPlayer(go);
				break;
			}
		}
	}

	server->UpdateServer();

	// Send Object Updates to Clients
	for (auto o : objects) {
		GameObject* go = o->GetGameObject();
		if (!go) continue;

		Transform t = go->GetTransform();
		Vector3 pos = t.GetWorldPosition();
		Quaternion rot = t.GetWorldOrientation();
		Vector3 sca = t.GetWorldScale();
		bool ia = go->IsActive();

		RenderObject* ro = go->GetComponent<RenderObject*>();
		Vector3 col;
		if (ro) {
			Vector4 originalCol = ro->GetMaterial()->GetColour();
			col = Vector3(originalCol.x, originalCol.y, originalCol.z);
		}

		size_t hashed = std::hash<std::string>{}(ToString(pos) + ToString(rot) + ToString(sca) + ToString(col) + (ia ? '1' : '0'));
		if (hashed == o->GetHash()) continue;

		o->SetHash(hashed);
		ObjectUpdatePacket p = ObjectUpdatePacket(o->GetId(), pos, rot, sca, col, ia);
		server->SendGlobalPacket(p);
	}

	// Send Player State to Clients
	for (auto player : players) {
		PlayerState ps = PlayerState(
			player->gameObject->GetComponent<Player*>()->GetHP(),
			player->gameObject->GetComponent<Player*>()->GetResourceCount(),
			player->gameObject->GetComponent<NetworkObject*>()->GetId()
		);

		if (player->peerId == -1) {
			playerState = ps;
		}
		else {
			PlayerStatePacket p = PlayerStatePacket(ps);
			server->SendPacket(p, player->peerId);
		}
	}
}

void NetworkServer::OnClientConnect(int source) {

	for (auto o : objects) {
		Transform t = o->GetGameObject()->GetTransform();
		Vector3 pos = t.GetWorldPosition();
		Quaternion rot = t.GetWorldOrientation();
		Vector3 sca = t.GetLocalScale();
		bool ia = o->GetGameObject()->IsActive();

		InstantiatePacket p = InstantiatePacket(o->GetPrefabId(), o->GetId(), pos, rot, sca, ia);
		server->SendPacket(p, source);
	}

	auto player = new PlayerPrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	world->LateInstantiateRecursively(player);

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
	}
	else if (type == PlayerInputMessage)
	{
		PlayerInputPacket* realPacket = (PlayerInputPacket*)payload;

		PlayerInput* ps = FindPlayer(source);
		ps->keysDown = realPacket->keysDown;
		ps->keysPressed = realPacket->keysPressed;
		ps->cameraPosition = realPacket->cameraPosition;
		ps->cameraRotation = realPacket->cameraRotation;
	}
}

void NetworkServer::Instantiate(GameObject* go) {
	if (!go) return;

	NetworkObject* no = go->GetComponent<NetworkObject*>();
	if (!no) return;

	no->SetId(lastestId++);
	objects.push_back(no);

	server->SendGlobalPacket(InstantiatePacket(no->GetPrefabId(), no->GetId(), go->GetTransform().GetWorldPosition(), go->GetTransform().GetLocalOrientation(), go->GetTransform().GetLocalScale()));
}

void NetworkServer::Destroy(GameObject * go)
{
	for (auto childTransform : go->GetTransform().GetChildrenList()) {
		Destroy(childTransform->GetGameObject());
	}

	NetworkObject* no = go->GetComponent<NetworkObject*>();
	if (no) {
		server->SendGlobalPacket(DestroyPacket(no->GetId()));
		objects.erase(remove(objects.begin(), objects.end(), no), objects.end());
	}
	world->LateDestroy(go);
}

void NetworkServer::AddPlayer(int peerId, GameObject* go) {
	players.push_back(new PlayerInput(peerId, go));
}

void NetworkServer::RemovePlayer(int peerId) {
	PlayerInput* ps = FindPlayer(peerId);
	if (!ps) return;

	Destroy(ps->gameObject);
	players.erase(remove(players.begin(), players.end(), ps), players.end());
}

PlayerInput* NetworkServer::FindPlayer(int peerId) {
	for (auto p : players) {
		if (p->peerId == peerId) return p;
	}
	return nullptr;
}

PlayerInput* NetworkServer::FindPlayer(GameObject* go) {
	for (auto p : players) {
		if (p->gameObject == go) return p;
	}
	return nullptr;
}