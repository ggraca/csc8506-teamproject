#include "ConePrefab.h"

ConePrefab::ConePrefab()
{
	OBJGeometry* coneMesh = Assets::AssetManager::LoadOBJ("cone.obj");
	GetTransform().SetWorldScale(Vector3(1, 1, 1));
	GetTransform().SetWorldPosition(Vector3(0, 0, 0));
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cone, 10));
	/*AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cone.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();*/
}

ConePrefab::ConePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction)
{
	OBJGeometry* coneMesh = Assets::AssetManager::LoadOBJ("cone.obj");
	GameObject* cone = GameObject::FromOBJ(coneMesh);

	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&cone->GetTransform(), ShapeType::cone, mass, restitution, friction));
	/*AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cone.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();*/
}


ConePrefab::~ConePrefab()
{
}
