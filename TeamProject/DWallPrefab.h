#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"
#include "../Common/Material.h"
#include "../Common/Assets.h"
#include "OGLShader.h"

using namespace NCL;
using namespace NCL::CSC8503;

class DWallPrefab : public GameObject
{
public:
	DWallPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient);
	virtual ~DWallPrefab();
	void SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient);
};
