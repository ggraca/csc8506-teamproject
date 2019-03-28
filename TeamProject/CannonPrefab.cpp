#include "CannonPrefab.h"
#include "Destructible.h"
#include "CannonScript.h"

CannonPrefab::CannonPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	//OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("14054_Pirate_Ship_Cannon_on_Cart_v1_l3.obj");
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("cannon.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Cannon));
	SetTransformDetails(Vector3(60.0f, 50.0f, 45.0f), Position, orient);
	go->AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, 0.0f, 1.0f, 1.0f));
	GameObject::gameWorld->AddObjectPhysicsToWorld(go->GetComponent<PhysicsObject*>());
	go->AddComponent<HealthManager*>(new HealthManager(go, 20));
	go->AddComponent<Destructible*>(new Destructible(go));
	go->AddComponent<CannonScript*>(new CannonScript(go));
}

CannonPrefab::~CannonPrefab()
{
}

void CannonPrefab::SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
}