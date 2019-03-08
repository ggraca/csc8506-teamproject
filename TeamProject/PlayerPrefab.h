#pragma once
#include "GameObject.h"
#include "Player.h"
#include "CubePrefab.h"

using namespace NCL;
using namespace CSC8503;

class PlayerPrefab : public CubePrefab
{
public:
	PlayerPrefab();
	void ResetPlayer();
	PlayerPrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction);
	virtual ~PlayerPrefab();
};
