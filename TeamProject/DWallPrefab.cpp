#include "DWallPrefab.h"

DWallPrefab::DWallPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "full_wall.obj";
//	string filename = "oldWall.obj";
	Material* mat = Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"));

	AddRenderObject(filename, this, dimensions, position, orient, mat);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::DWall));
}

DWallPrefab::~DWallPrefab()
{
	/*if (mat != nullptr) {
		delete mat;
	}*/	
}