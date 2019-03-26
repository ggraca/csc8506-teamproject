#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"
#include "../Common/Assets.h"
#include "OGLShader.h"


using namespace NCL;
using namespace CSC8503;

class SpherePrefab : public GameObject
{
public:
	SpherePrefab();
	SpherePrefab(const Vec3& position, float radius, float mass, float restitution, float friction);
	virtual ~SpherePrefab();
};

