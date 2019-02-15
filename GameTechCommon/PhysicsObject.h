#pragma once
#include "../Common/Vector3.h"
#include "../Common/Matrix3.h"
#include "../Common/Matrix3.h"

//using namespace NCL::Maths; //TODO Remove this?

class PhysicsObject	{
public:
	PhysicsObject(Transform* parentTransform);
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
		Transform* transform;

		float mass;
		float restitution;
		float friction;
};


