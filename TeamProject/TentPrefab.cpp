#include "TentPrefab.h"

TentPrefab::TentPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	string fileName = "Market.obj";
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ(fileName);
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Tent));
	std::ifstream infile(Assets::MESHDIR + "x_Market.obj");
	if (infile)
	{
		objGeometry = Assets::AssetManager::LoadOBJ("x_Market.obj");
	}
	go->AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&go->GetTransform(), ShapeType::complexMesh, 0, 0, 0, fileName, objGeometry, true));
}

TentPrefab::~TentPrefab()
{
}