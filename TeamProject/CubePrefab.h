#pragma once
#include "GameObject.h"
#include "../Common/Material.h"

using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Rendering;

class CubePrefab : public GameObject
{
public:
	CubePrefab();
	CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction);
	~CubePrefab();
};

