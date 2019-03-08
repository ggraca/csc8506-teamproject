#include "SpherePrefab.h"

SpherePrefab::SpherePrefab()
{
	GetTransform().SetWorldScale(Vector3(1, 1, 1));
	GetTransform().SetWorldPosition(Vector3(0,0,0));
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::sphere, 10, 0.8f, 0.4f));
	OGLShader* basicShader = new OGLShader("pbrvert.glsl", "pbrfrag.glsl");
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("CubeMesh"), Assets::AssetManager::LoadMaterial("Basic Material", basicShader)));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}

SpherePrefab::SpherePrefab(const Vector3& position, float radius, float mass, float restitution, float friction)
{
	GetTransform().SetWorldScale(Vector3(radius, radius, radius));
	GetTransform().SetWorldPosition(position);
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::sphere, mass, restitution, friction));
	OGLShader* basicShader = new OGLShader("pbrvert.glsl", "pbrfrag.glsl");
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("CubeMesh"), Assets::AssetManager::LoadMaterial("Basic Material", basicShader)));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}


SpherePrefab::~SpherePrefab()
{
}
