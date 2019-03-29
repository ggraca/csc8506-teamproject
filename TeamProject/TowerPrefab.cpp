#include "TowerPrefab.h"
#include "Destructible.h"

TowerPrefab::TowerPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "War_Tower.obj";
	Material* mat = Assets::AssetManager::GetMaterial("Wall Material");

	AddRenderObject(filename, this, dimensions, position, orient, mat);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Tower));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 30));
	AddComponent<Destructible*>(new Destructible(this));
}

TowerPrefab::~TowerPrefab()
{
}