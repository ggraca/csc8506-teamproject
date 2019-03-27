#include "MarketPrefab.h"
#include "Destructible.h"

MarketPrefab::MarketPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "market-stand.obj";

	AddRenderObject(filename, this, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Market));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 30));
	AddComponent<Destructible*>(new Destructible(this));
}

MarketPrefab::~MarketPrefab()
{
}