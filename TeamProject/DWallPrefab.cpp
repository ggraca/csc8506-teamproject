#include "DWallPrefab.h"
#include "Destructible.h"

DWallPrefab::DWallPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("full_wall.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::DWall));
	SetTransformDetails(Vector3(400.0f, 150.0f, 45.0f), Position, orient);
	go->AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, 0.0f, 1.0f, 1.0f));
	GameObject::gameWorld->AddObjectPhysicsToWorld(go->GetComponent<PhysicsObject*>());
	go->AddComponent<HealthManager*>(new HealthManager(go, 30));
	go->AddComponent<Destructible*>(new Destructible(go));
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