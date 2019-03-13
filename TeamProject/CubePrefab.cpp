#include "CubePrefab.h"


CubePrefab::CubePrefab()
{
	SetTransformDetails(Vector3(1,1,1), Vector3(0, 0, 0), Quaternion::EulerAnglesToQuaternion(0, 0, 0));
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, 10));
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader","pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}

CubePrefab::CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction)
{
	SetTransformDetails(dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component *)new PhysicsObject(&GetTransform(), ShapeType::cube, mass, restitution, friction));
	AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("PBRShader", "pbrvert.glsl", "pbrfrag.glsl"))));
	GetComponent<RenderObject*>()->SetMaterialInstanced();
}

CubePrefab::CubePrefab(CubePrefab::PrefabType type)
{
	switch (type)
	{
	case CubePrefab::GUN: // I didn't want to create a prefab class just for these lines.
		SetTransformDetails(Vector3(0.4f, 0.4f, 2.5f), Vector3(0, 0, 0), Quaternion::EulerAnglesToQuaternion(0, 0, 0));
		AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
		GetComponent<RenderObject*>()->SetMaterialInstanced();
		return;
	case CubePrefab::HANDLE:
		SetTransformDetails(Vector3(0.2f, 2.5f, 0.2f), Vector3(0, 0, 0), Quaternion::EulerAnglesToQuaternion(0, 0, 45.0f));
		AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
		GetComponent<RenderObject*>()->SetMaterialInstanced();
		return;
	}
}

void CubePrefab::SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
}

CubePrefab::~CubePrefab()
{
}
