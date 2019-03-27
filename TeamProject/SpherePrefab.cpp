#include "SpherePrefab.h"

SpherePrefab::SpherePrefab()
{
	GetTransform().SetWorldScale(Vector3(1, 1, 1));
	GetTransform().SetWorldPosition(Vector3(0,0,0));
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::sphere, 10));
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("sphere.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}

SpherePrefab::SpherePrefab(const Vector3& position, float radius, float mass, float restitution, float friction)
{
	
	this->GetTransform().SetWorldScale(Vector3(radius, radius, radius));
	this->GetTransform().SetWorldPosition(position);
	this->AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::sphere, mass, restitution, friction));
	this->AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("sphere.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
	this->GetComponent<RenderObject*>()->SetMaterialInstanced();
}


SpherePrefab::~SpherePrefab()
{
}
