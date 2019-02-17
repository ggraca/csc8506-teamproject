#pragma once
#include "../Common/Vector3.h"
#include "../Common/Matrix3.h"

#include "BulletPhysics.h"
#include "C:\Users\Ed\Documents\GitHub\csc8506-teamproject\TeamProject\Scene.h"
#include "C:\Users\Ed\Documents\GitHub\csc8506-teamproject\GameTechCommon\Transform.h"

//using namespace NCL::Maths; //TODO Remove this?

class Transform;
//class Scene;

class PhysicsObject	{
public:
	PhysicsObject(NCL::CSC8503::Transform* parentTransform);
	~PhysicsObject();

	void SetMass(float mass) {
		this->mass = mass;
	}

	float GetMass() const {
		return mass;
	}

	void SetRestitution(float restitution) {
		this->restitution = restitution;
	}

	float GetRestitution() const {
		return restitution;
	}

	void SetFriction(float friction) {
		this->friction = friction;
	}

	float GetFriction() const {
		return friction;
	}

protected:
	NCL::CSC8503::Transform* transform;

	Vector3 position;
	Quaternion orientation;
	Vector3 dimensions;
	float mass;
	float restitution;
	float friction;
};


