#pragma once
#include "GameObject.h"
#include "Player.h"
#include "CubePrefab.h"
#include "GunControl.h"
#include "HammerControl.h"
#include "ShieldControl.h"
#include "AirStrikeControl.h"
#include "BigGunControl.h"


using namespace NCL;
using namespace CSC8503;

class PlayerPrefab : public CubePrefab
{
public:
	PlayerPrefab();
	void ResetPlayer();
	PlayerPrefab(const Vec3& position, const Quaternion& orient, Vec3 dimensions, float mass, float restitution, float friction);
	virtual ~PlayerPrefab();
	void InitializeGuns(GameObject * player);
	void InitializeShield(GameObject * player);
	void InitializeBigGun(GameObject * player);
	void InitializeHammer(GameObject * player);

	static Vec4 colours[];
	static int currentColour;
	static Vec4 GetNextColour() {
		Vec4 colour = PlayerPrefab::colours[PlayerPrefab::currentColour];
		PlayerPrefab::currentColour++;
		if (PlayerPrefab::currentColour > 3) PlayerPrefab::currentColour = 0;
		return colour;
	}
};