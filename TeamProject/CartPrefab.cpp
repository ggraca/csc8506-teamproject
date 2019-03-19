#include "CartPrefab.h"

CartPrefab::CartPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	string fileName = "WV Cart 01.obj";
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("WV Cart 01.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Cart));
	go->AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&go->GetTransform(), ShapeType::complexMesh, 0, 0, 0, fileName, objGeometry, false));
}

CartPrefab::~CartPrefab()
{
}