#include <string>

#include "../Common/Utils.h"
#include "NetworkServer.h"
#include "NetworkPackets.h"


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
}

void NetworkServer::OnClientConnect(int source) {
	cout << ToString(Vector3(1, 0, 6)) << endl;
	cout << ToString(Vector3(1, 4, 0)) << endl;
	cout << ToString(Vector3(0, 4, 6)) << endl;
	for (auto o : objects) {
		Transform t = o->GetGameObject()->GetTransform();
		Vector3 pos = t.GetWorldPosition();
		Quaternion rot = t.GetLocalOrientation();

		InstantiatePacket p = InstantiatePacket(o->GetPrefabId(), o->GetId(), pos, rot);
		server->SendGlobalPacket(p);
	}
}

void NetworkServer::OnClientDisconnect(int source) {
	cout << "LAWL" << endl;
}

void NetworkServer::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == StringMessage)
	{
		StringPacket* realPacket = (StringPacket*)payload;
		string msg = realPacket->GetStringFromData();
		std::cout << "received message: " << msg << std::endl;
	}
	else if (type == PlayerPos)
	{
		PlayerPosPacket* realPacket = (PlayerPosPacket*)payload;
		float* pos = realPacket->GetPosFromData();
		std::cout << "X: " << pos[0] << " Y: " << pos[1] << " Z: " << pos[2] << std::endl;
	}
}

void NetworkServer::Instantiate(GameObject* go) {
	if (!go) return;

	NetworkObject* no = go->GetComponent<NetworkObject*>();
	if (!no) return;

	no->SetId(lastestId++);
	objects.push_back(no);

	server->SendGlobalPacket(StringPacket("yo"));
}