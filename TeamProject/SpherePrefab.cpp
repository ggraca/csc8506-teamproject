#include "SpherePrefab.h"
#include "PhysicsObject.h"


SpherePrefab::SpherePrefab(const Vector3& position, float radius, float mass, float restitution, float friction) 
{
	GetTransform().SetWorldScale(Vector3(radius, radius, radius));
	GetTransform().SetWorldPosition(position);
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::sphere, mass, restitution, friction));
}


SpherePrefab::~SpherePrefab()
{
}
