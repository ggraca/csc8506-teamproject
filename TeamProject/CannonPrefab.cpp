#include "CannonPrefab.h"

CannonPrefab::CannonPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	//OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("14054_Pirate_Ship_Cannon_on_Cart_v1_l3.obj");
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("cannon.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Cannon));
}

CannonPrefab::~CannonPrefab()
{
}