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
	TentPrefab(const Vec3& Scale, const Vec3& Position, const Quaternion& orient);
	virtual ~TentPrefab();
};

