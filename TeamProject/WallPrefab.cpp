#include "WallPrefab.h"
#include "Destructible.h"

WallPrefab::WallPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "oldWall.obj";

	AddRenderObject(filename, this, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Wall));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 400));
	AddComponent<Destructible*>(new Destructible(this));



	/*OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("oldWall.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(dimensions);
	go->GetTransform().SetWorldPosition(position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Wall));
	SetTransformDetails(Vector3(150.0f,100.0f,20.0f), position, orient);
	go->AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, 0.0f, 1.0f, 1.0f));
	GameObject::gameWorld->AddObjectPhysicsToWorld(go->GetComponent<PhysicsObject*>());
	go->AddComponent<HealthManager*>(new HealthManager(go, 400));
	go->AddComponent<Destructible*>(new Destructible(go));*/
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