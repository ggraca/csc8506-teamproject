#include "WellPrefab.h"
#include "Destructible.h"

WellPrefab::WellPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "new_well.obj";

	AddRenderObject(filename, this, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Well));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 30));
	AddComponent<Destructible*>(new Destructible(this));
	GameObject::gameWorld->AddObjectPhysicsToWorld(GetComponent<PhysicsObject*>());
}

WellPrefab::~WellPrefab()
{
}