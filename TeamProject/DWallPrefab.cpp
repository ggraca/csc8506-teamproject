#include "DWallPrefab.h"

DWallPrefab::DWallPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	string fileName = "full_wall.obj";
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ(fileName);
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::DWall));
	go->AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&go->GetTransform(), ShapeType::complexMesh, 0, 0, 0, fileName, objGeometry, true));
}

DWallPrefab::~DWallPrefab()
{
}