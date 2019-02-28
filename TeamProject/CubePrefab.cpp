#include "CubePrefab.h"
#include "PhysicsObject.h"

CubePrefab::CubePrefab() 
{
	CubePrefab(Vector3(0, 0, 0), Quaternion::EulerAnglesToQuaternion(0, 0, 0), Vector3(1, 1, 1), 10, 0.8f, 0.4f);
}

CubePrefab::CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
	//AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, mass, restitution, friction));
}


CubePrefab::~CubePrefab()
{
}
