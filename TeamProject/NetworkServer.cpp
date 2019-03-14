#include "NetworkServer.h"
#include "NetworkPackets.h"

void NetworkServer::OnClientConnect(int source) {
	for (auto o : objects) {
		Vector3 pos = o->GetGameObject()->GetTransform().GetWorldPosition();
		InstantiatePacket p = InstantiatePacket(o->GetId(), 0, pos.x, pos.y, pos.z);
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

	objects.push_back(no);
	no->SetId(lastestId++);

	server->SendGlobalPacket(StringPacket("yo"));
}