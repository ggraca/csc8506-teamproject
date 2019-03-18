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
	enum PrefabType{GUN,HANDLE,HAMMER_HEAD,SHIELD};
	CubePrefab();
	CubePrefab(const Vector3& position, const Quaternion& orient, Vector3 dimensions, float mass, float restitution, float friction);
	void SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient);
	CubePrefab(CubePrefab::PrefabType type);
	virtual ~CubePrefab();
};

