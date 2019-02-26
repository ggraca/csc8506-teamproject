#include "PlayerPrefab.h"



PlayerPrefab::PlayerPrefab()
{
	auto player = new CubePrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	player->AddScript((ScriptObject*)new Player(player));
	player->SetTag(LayerAndTag::Tags::Player);
	player->GetPhysicsObject()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	
}


PlayerPrefab::~PlayerPrefab()
{
}
