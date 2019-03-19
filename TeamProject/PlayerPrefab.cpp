#include "PlayerPrefab.h"


PlayerPrefab::PlayerPrefab() :CubePrefab()
{
	// TODO Set transform values if needed.
	ResetPlayer();
}

PlayerPrefab::PlayerPrefab(const Vector3 & position, const Quaternion & orient, Vector3 dimensions, float mass, float restitution, float friction)
:CubePrefab(position,orient,dimensions,mass,restitution,friction)
{
	ResetPlayer();
}

PlayerPrefab::~PlayerPrefab()
{
}

void PlayerPrefab::ResetPlayer()
{
	AddComponent<Player*>((Component*)new Player(this));
	AddComponent<GunControl*>(new GunControl(this));
	AddComponent<HammerControl*>(new HammerControl(this));
	AddComponent<ShieldControl*>(new ShieldControl(this));
	GetComponent<HammerControl*>()->SetHammerSize(3000, 0, 500, 3000, 0, 3000);
	SetTag(LayerAndTag::Tags::Player);
	GetComponent<PhysicsObject*>()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	GetComponent<RenderObject*>()->GetMaterial()->SetColour(Vector4(1, 0, 0, 1));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Player));
}