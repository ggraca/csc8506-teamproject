#include "CubePrefab.h"


CubePrefab::CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction)
{
	GameObject* cube = new GameObject();

	cube->GetTransform().SetWorldScale(dimensions);
	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetLocalOrientation(orient);
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), ShapeType::cube, mass, restitution, friction));
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicMaterial));
}


CubePrefab::~CubePrefab()
{
}
