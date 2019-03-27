#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"
#include "../Common/Assets.h"
#include "OGLShader.h"


using namespace NCL;
using namespace CSC8503;

class ConePrefab : public GameObject
{
public:
	ConePrefab(const Vector3 dimensions, const Vector3& position, const Quaternion& orient, float mass, float restitution, float friction);
	virtual ~ConePrefab();
};

