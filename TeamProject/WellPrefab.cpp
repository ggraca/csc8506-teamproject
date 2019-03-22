#include "WellPrefab.h"
#include "Destructible.h"
#include "HealthManager.h"

WellPrefab::WellPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("new_well.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Well));

	go->AddComponent<PhysicsObject*>(new PhysicsObject(&(go->GetTransform()), ShapeType::cube, 10));
	go->AddComponent<Destructible*>(new Destructible(go));
	go->AddComponent<HealthManager*>(new HealthManager(go));
	go->GetComponent<HealthManager*>()->SetHealth(8);
}

WellPrefab::~WellPrefab()
{
}