#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

class CubePrefab : virtual public GameObject
{
public:
	CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction);
	~CubePrefab();

protected:
	MeshGeometry* cubeMesh;
	Material* basicMaterial;
};

