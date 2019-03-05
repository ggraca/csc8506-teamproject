#include "PlayerPrefab.h"


PlayerPrefab::PlayerPrefab() :CubePrefab()
{
	//TODO Set transform values if needed.
	ResetPlayer();
}

void PlayerPrefab::ResetPlayer()
{
	AddComponent<Player*>((Component*)new Player(this));
	SetTag(LayerAndTag::Tags::Player);
	GetComponent<PhysicsObject*>()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
}

PlayerPrefab::PlayerPrefab(const Vector3 & position, const Quaternion & orient, Vector3 dimensions, float mass, float restitution, float friction): CubePrefab(position,orient,dimensions,mass,restitution,friction)
{
	ResetPlayer();
}

PlayerPrefab::~PlayerPrefab()
{
}
