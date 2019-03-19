#include "CannonPrefab.h"

CannonPrefab::CannonPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	string fileName = "14054_Pirate_Ship_Cannon_on_Cart_v1_l3.obj";
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ(fileName);
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Cannon));
	go->AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&go->GetTransform(), ShapeType::complexMesh, 0, 0, 0, fileName, objGeometry, false));
}

CannonPrefab::~CannonPrefab()
{
}