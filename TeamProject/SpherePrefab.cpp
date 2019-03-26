#include "SpherePrefab.h"

SpherePrefab::SpherePrefab()
{
	GetTransform().SetWorldScale(Vec3(1, 1, 1));
	GetTransform().SetWorldPosition(Vec3(0,0,0));
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::sphere, 10));
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("sphere.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}

SpherePrefab::SpherePrefab(const Vec3& position, float radius, float mass, float restitution, float friction)
{
	GetTransform().SetWorldScale(Vec3(radius, radius, radius));
	GetTransform().SetWorldPosition(position);
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::sphere, mass, restitution, friction));
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("sphere.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}


SpherePrefab::~SpherePrefab()
{
}
