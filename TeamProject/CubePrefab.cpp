#include "CubePrefab.h"


CubePrefab::CubePrefab()
{
	GetTransform().SetWorldScale(Vector3(1, 1, 1));
	GetTransform().SetWorldPosition(Vector3(0, 0, 0));
	GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, 10));
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader","pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}

CubePrefab::CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, mass, restitution, friction));
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("PBRShader", "pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}

CubePrefab::~CubePrefab()
{
}
