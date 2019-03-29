#include "ConePrefab.h"

ConePrefab::ConePrefab(const Vector3 dimensions, const Vector3& position, const Quaternion& orient, float mass, float restitution, float friction)
{
	string filename = "cone.obj";
	Material* mat = Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"));

	AddRenderObject(filename, this, dimensions, position, orient, mat);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::cone, mass, restitution, friction, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Cone));
}

ConePrefab::~ConePrefab()
{
}