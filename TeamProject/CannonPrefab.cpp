#include "CannonPrefab.h"
#include "Destructible.h"
#include "CannonScript.h"

CannonPrefab::CannonPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "cannon.obj";

	AddRenderObject(filename, this, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Cannon));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 20));
	AddComponent<Destructible*>(new Destructible(this));
	AddComponent<CannonScript*>(new CannonScript(this));
	GameObject::gameWorld->AddObjectPhysicsToWorld(GetComponent<PhysicsObject*>());
}

CannonPrefab::~CannonPrefab()
{
}
