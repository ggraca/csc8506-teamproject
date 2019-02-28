#pragma once
#include "GameObject.h"


using namespace NCL;
using namespace CSC8503;

class SpherePrefab : public GameObject
{
public:
	SpherePrefab(const Vector3& position, float radius, float mass, float restitution, float friction);
	~SpherePrefab();
};

