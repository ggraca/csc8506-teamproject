#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"
#include "../Common/Material.h"
#include "../Common/Assets.h"
#include "OGLShader.h"

using namespace NCL;
using namespace NCL::CSC8503;

class CartPrefab : public GameObject
{
public:
	CartPrefab(const Vec3& Scale, const Vec3& Position, const Quaternion& orient);
	virtual ~CartPrefab();
};
