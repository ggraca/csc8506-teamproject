#include "CubePrefab.h"
#include "DamageControl.h"

CubePrefab::CubePrefab()
{
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 pos = Vector3(0, 0, 0);
	Quaternion rot = Quaternion::EulerAnglesToQuaternion(0, 0, 0);

	SetTransformDetails(scale, pos, rot);
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
	Vector3 scale;
	Vector3 pos;
	Quaternion rot;

	switch (type)
	{
	case CubePrefab::PrefabType::GUN: // I didn't want to create a prefab class just for these lines.
		scale = Vector3(0.4f, 0.4f, 2.5f);
		pos = Vector3(0, 0, 0);
		rot = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
		SetTransformDetails(scale, pos, rot);
		AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
		GetComponent<RenderObject*>()->SetMaterialInstanced();
		break;
	case CubePrefab::PrefabType::HANDLE:
		scale = Vector3(0.2f, 2.5f, 0.2f);
		pos = Vector3(0, 0, 0);
		rot = Quaternion::EulerAnglesToQuaternion(0, 0, 45.0f);
		SetTransformDetails(scale, pos, rot);
		AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
		GetComponent<RenderObject*>()->SetMaterialInstanced();
		break;
	case CubePrefab::PrefabType::SHIELD:
		scale = Vector3(30.0f, 30.0f, 5.0f);
		pos = Vector3(0, 0, 0);
		rot = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
		SetTransformDetails(scale, pos, rot);
		break;
	case CubePrefab::PrefabType::BIG_GUN:
		scale = Vector3(1.5f, 4.0f, 1.5f);
		pos = Vector3(0, 0, 0);
		rot = Quaternion::EulerAnglesToQuaternion(90.0f, 0, 0);
		SetTransformDetails(scale, pos, rot);
		AddComponent<RenderObject*>(new RenderObject(&GetTransform(), Assets::AssetManager::LoadMesh("Cube.msh"), Assets::AssetManager::LoadMaterial("Basic Material", Assets::AssetManager::LoadShader("basicShader", "pbrvert.glsl", "pbrfrag.glsl"))));
		GetComponent<RenderObject*>()->SetMaterialInstanced();
		GetComponent<RenderObject*>()->GetMaterial()->SetColour(Vector4(1, 0, 0, 1));
		break;
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
