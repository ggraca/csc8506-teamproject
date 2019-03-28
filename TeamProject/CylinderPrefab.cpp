#include "CylinderPrefab.h"

CylinderPrefab::CylinderPrefab(const Vector3 dimensions, const Vector3& position, const Quaternion& orient, float mass, float restitution, float friction)
{
	string filename = "cylinder.obj";
	Material* mat = Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"));

	AddRenderObject(filename, this, dimensions, position, orient, mat);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::cylinder, mass, restitution, friction, filename));
	gameWorld->AddObjectPhysicsToWorld(this->GetComponent<PhysicsObject*>());
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Cylinder));
	GameObject::gameWorld->AddObjectPhysicsToWorld(GetComponent<PhysicsObject*>());
}

CylinderPrefab::~CylinderPrefab()
{
}
