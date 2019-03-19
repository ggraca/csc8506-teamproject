#include "StallPrefab.h"

StallPrefab::StallPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	string fileName = "vws.obj";
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ(fileName);
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Stall));
	go->AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&go->GetTransform(), ShapeType::complexMesh, 0, 0, 0, fileName, objGeometry, false));
}

StallPrefab::~StallPrefab()
{
}