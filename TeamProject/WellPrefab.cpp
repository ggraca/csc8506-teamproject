#include "WellPrefab.h"
#include "Destructible.h"

WellPrefab::WellPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("new_well.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Well));
	SetTransformDetails(Vector3(40.0f, 100.0f, 40.0f), Position, orient);
	go->AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, 0.0f, 1.0f, 1.0f));
	GameObject::gameWorld->AddObjectPhysicsToWorld(go->GetComponent<PhysicsObject*>());
	go->AddComponent<HealthManager*>(new HealthManager(go, 200));
	go->AddComponent<Destructible*>(new Destructible(go));
}

WellPrefab::~WellPrefab()
{
}

void WellPrefab::SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
}