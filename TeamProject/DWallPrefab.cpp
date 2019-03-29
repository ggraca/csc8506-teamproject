#include "DWallPrefab.h"
#include "Destructible.h"

DWallPrefab::DWallPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "full_wall.obj";
	Material* mat = Assets::AssetManager::GetMaterial("Wall Material");

	AddRenderObject(filename, this, dimensions, position, orient, mat);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::DWall));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 30));
	AddComponent<Destructible*>(new Destructible(this));
}

DWallPrefab::~DWallPrefab()
{
}
