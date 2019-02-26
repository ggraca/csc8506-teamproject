#include "SpherePrefab.h"



SpherePrefab::SpherePrefab(const Vector3& position, float radius, float mass, float restitution, float friction) 
{
	GameObject* sphere = new GameObject();

	sphere->GetTransform().SetWorldScale(Vector3(radius, radius, radius));
	sphere->GetTransform().SetWorldPosition(position);
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), ShapeType::sphere, mass, restitution, friction));
	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicMaterial));
}


SpherePrefab::~SpherePrefab()
{
}
