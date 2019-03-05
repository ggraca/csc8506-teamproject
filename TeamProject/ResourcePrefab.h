#pragma once
#include "Resource.h"
#include "CubePrefab.h"

using namespace NCL;
using namespace CSC8503;

class ResourcePrefab : public CubePrefab
{
public:
	ResourcePrefab();
	ResourcePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction);
	virtual ~ResourcePrefab();
};

