#include "CartPrefab.h"
#include "Destructible.h"

CartPrefab::CartPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "WV Cart 01.obj";

	AddRenderObject(filename, this, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Cart));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 200));
	AddComponent<Destructible*>(new Destructible(this));
}

CartPrefab::~CartPrefab()
{
}
