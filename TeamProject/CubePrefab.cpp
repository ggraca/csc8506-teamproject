#include "CubePrefab.h"


CubePrefab::CubePrefab() {};

CubePrefab::CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
	SetPhysicsObject(new PhysicsObject(&GetTransform(), ShapeType::cube, mass, restitution, friction));
}


CubePrefab::~CubePrefab()
{
}
