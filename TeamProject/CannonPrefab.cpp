#include "CannonPrefab.h"

CannonPrefab::CannonPrefab(const Vec3& Scale, const Vec3& Position, const Quaternion& orient)
{
	//OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("14054_Pirate_Ship_Cannon_on_Cart_v1_l3.obj");
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("cannon.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
#ifdef _WIN32
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Cannon));
#endif
}

CannonPrefab::~CannonPrefab()
{
}