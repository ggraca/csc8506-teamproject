#include "DWallPrefab.h"
#include "Destructible.h"

DWallPrefab::DWallPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "full_wall.obj";
	AddRenderObject(filename, this, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	GameObject::gameWorld->AddObjectPhysicsToWorld(this->GetComponent<PhysicsObject*>());
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::DWall));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 1));
	AddComponent<Destructible*>(new Destructible(this));
}

DWallPrefab::~DWallPrefab()
{
}