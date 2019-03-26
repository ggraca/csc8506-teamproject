#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"
#include "../Common/Material.h"
#include "../Common/Assets.h"
#include "OGLShader.h"

using namespace NCL;
using namespace NCL::CSC8503;

class TentPrefab : public GameObject
{
public:
	TentPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient);
	virtual ~TentPrefab();
	void SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient);
};

