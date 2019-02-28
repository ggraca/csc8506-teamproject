#include "PlayerPrefab.h"
#include "PhysicsObject.h"


PlayerPrefab::PlayerPrefab()
{
	auto player = new CubePrefab(Vector3(120, 260, 50), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(10, 10, 10), 100, 0.2f, 0.4f);
	AddComponent<Player*>((Component*)new Player(player));
	SetTag(LayerAndTag::Tags::Player);
	GetComponent<PhysicsObject*>()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
}


PlayerPrefab::~PlayerPrefab()
{
}
