#include "WellPrefab.h"

WellPrefab::WellPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	string fileName = "well.obj";
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ(fileName);
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
	go->AddComponent<NetworkObject*>(new NetworkObject(go, NetworkObject::Well));
	std::ifstream infile(Assets::MESHDIR + "x_well.obj");
	if (infile)
	{
		objGeometry = Assets::AssetManager::LoadOBJ("x_well.obj");
	}
	go->AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&go->GetTransform(), ShapeType::complexMesh, 0, 0, 0, fileName, objGeometry, true));
}

WellPrefab::~WellPrefab()
{
}