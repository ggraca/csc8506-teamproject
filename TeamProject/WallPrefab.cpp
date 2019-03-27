#include "WallPrefab.h"
#include "Destructible.h"

WallPrefab::WallPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("oldWall.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);

	for(auto children : go->GetTransform().GetChildrenList()) {
		children->GetGameObject()->SetName("Wall");
	}

	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Wall));
	SetTransformDetails(Vector3(150.0f,100.0f,20.0f), Position, orient);
	go->AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, 0.0f, 1.0f, 1.0f));
	GameObject::gameWorld->AddObjectPhysicsToWorld(go->GetComponent<PhysicsObject*>());
	go->AddComponent<HealthManager*>(new HealthManager(go, 400));
	go->AddComponent<Destructible*>(new Destructible(go));
}

WallPrefab::~WallPrefab()
{
}

void WallPrefab::SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
}