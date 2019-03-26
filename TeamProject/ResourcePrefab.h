#pragma once
#include "Resource.h"
#include "CubePrefab.h"
#include "DamageControl.h"

using namespace NCL;
using namespace CSC8503;

class ResourcePrefab : public CubePrefab
{
public:
	ResourcePrefab();
	ResourcePrefab(const Vec3& position, const Quaternion& orient, Vec3 dimensions, float mass, float restitution, float friction);
	virtual ~ResourcePrefab();
};

