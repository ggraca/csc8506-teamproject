#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"
#include "../Common/Material.h"
#include "../Common/Assets.h"
#include "OGLShader.h"

using namespace NCL;
using namespace NCL::CSC8503;

class CubePrefab : public GameObject
{
public:
	enum PrefabType{GUN};
	CubePrefab();
	CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction);
	CubePrefab(CubePrefab::PrefabType type);
	virtual ~CubePrefab();
};

