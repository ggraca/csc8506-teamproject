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
	enum PrefabType{GUN,HANDLE,HAMMER_HEAD,SHIELD,BIG_GUN};
	CubePrefab();
	CubePrefab(const Vec3& position, const Quaternion& orient, Vec3 dimensions, float mass, float restitution, float friction);
	void SetTransformDetails(NCL::Maths::Vec3 &dimensions, const NCL::Maths::Vec3 & position, const NCL::Maths::Quaternion & orient);
	CubePrefab(CubePrefab::PrefabType type);
	virtual ~CubePrefab();
};

