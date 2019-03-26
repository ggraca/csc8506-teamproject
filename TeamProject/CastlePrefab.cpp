#include "CastlePrefab.h"

CastlePrefab::CastlePrefab(const Vec3& Scale, const Vec3& Position, const Quaternion& orient)
{
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("bau.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
#ifdef _WIN32

	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Castle));
#endif
}

CastlePrefab::~CastlePrefab()
{
}