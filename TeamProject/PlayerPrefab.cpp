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

void PlayerPrefab::InitializeGuns(GameObject * player)
{
	auto  playerLeft = new CubePrefab(CubePrefab::PrefabType::GUN);
	auto  playerRight = new CubePrefab(CubePrefab::PrefabType::GUN);

	playerLeft->SetParent(player);
	playerRight->SetParent(player);

	playerRight->GetTransform().SetLocalPosition(Vector3(-2, 0, 1));
	playerLeft->GetTransform().SetLocalPosition(Vector3(2, 0, 1));

	player->GetComponent<GunControl*>()->SetRightGun(playerRight);
	player->GetComponent<GunControl*>()->SetLeftGun(playerLeft);
}

void PlayerPrefab::InitializeHammer(GameObject * player)
{
	auto  handle = new CubePrefab(CubePrefab::PrefabType::HANDLE);
	handle->SetParent(player);
	handle->GetTransform().SetLocalPosition(Vector3(-1, 0, 2));

	player->GetComponent<HammerControl*>()->SetHandle(handle);
}

void PlayerPrefab::InitializeShield(GameObject * player)
{
	auto shield = new CubePrefab(CubePrefab::PrefabType::SHIELD);
	GameObject * shieldDummy = new GameObject();
	shieldDummy->SetParent(player);
	shieldDummy->GetTransform().SetLocalPosition(Vector3(0, 2.5f, 6));

	player->GetComponent<ShieldControl*>()->SetShield(shield);
	player->GetComponent<ShieldControl*>()->SetTarget(&shieldDummy->GetTransform());
	player->GetComponent<ShieldControl*>()->SetShieldDummy(shieldDummy);
}

void PlayerPrefab::InitializeBigGun(GameObject * player)
{
	auto bigGun = new CubePrefab(CubePrefab::PrefabType::BIG_GUN);
	bigGun->SetParent(player);
	bigGun->GetTransform().SetLocalPosition(Vector3(-4, 2, 0));
	player->GetComponent<BigGunControl*>()->SetBigGun(bigGun);
}
void PlayerPrefab::ResetPlayer()
{
	AddComponent<Player*>((Component*)new Player(this));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Player));
	AddComponent<GunControl*>(new GunControl(this));
	AddComponent<HammerControl*>(new HammerControl(this));
	AddComponent<ShieldControl*>(new ShieldControl(this));
	AddComponent<AirStrikeControl*>(new AirStrikeControl(this));
	AddComponent<BigGunControl*>(new BigGunControl(this));
	GetComponent<HammerControl*>()->SetHammerSize(-5000, 5000, 800, 3000, -8000, 8000);
	SetTag(LayerAndTag::Tags::Player);
	GetComponent<PhysicsObject*>()->GetRigidbody()->setActivationState(DISABLE_DEACTIVATION);
	GetComponent<RenderObject*>()->GetMaterial()->SetColour(Vector4(1, 0, 0, 1));

	InitializeGuns(this);
	InitializeHammer(this);
	InitializeShield(this);
	InitializeBigGun(this);
}