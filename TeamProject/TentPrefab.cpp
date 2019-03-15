#include "TentPrefab.h"

TentPrefab::TentPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("Market.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Tent));
}

TentPrefab::~TentPrefab()
{
}