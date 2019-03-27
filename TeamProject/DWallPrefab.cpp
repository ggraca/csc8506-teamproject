#include "DWallPrefab.h"
#include "Destructible.h"

DWallPrefab::DWallPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "full_wall.obj";	
	Material* mat = Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"));

	AddRenderObject(filename, this, mat, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::DWall));
}

DWallPrefab::~DWallPrefab()
{
}

void DWallPrefab::SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
}