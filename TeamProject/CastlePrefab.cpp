#include "CastlePrefab.h"
#include "Destructible.h"

CastlePrefab::CastlePrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "bau.obj";
	Material* mat = Assets::AssetManager::GetMaterial("Basic Material");

	AddRenderObject(filename, this, dimensions, position, orient, mat);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Castle));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 200));
	AddComponent<Destructible*>(new Destructible(this));
}

CastlePrefab::~CastlePrefab()
{
}