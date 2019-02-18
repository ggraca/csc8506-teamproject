#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;


class CameraControl : virtual public ScriptObject
{
public:

	CameraControl(GameObject * obj);
	virtual ~CameraControl() {}

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void UpdateCamera();
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;

	float GetFieldOfVision() const {
		return fov;
	}

	float GetNearPlane() const {
		return nearPlane;
	}

	float GetFarPlane() const {
		return farPlane;
	}

	void SetNearPlane(float val) {
		nearPlane = val;
	}

	void SetFarPlane(float val) {
		farPlane = val;
	}

	Matrix4 BuildViewMatrix() const;
	Matrix4 BuildProjectionMatrix(float currentAspect = 1.0f) const;

	void SetPlayer(GameObject * obj);
	void FollowPlayer();
	void RotatePlayer();

protected:
	GameObject * player;

private:

	float	nearPlane;
	float	farPlane;
	float	left;
	float	right;
	float	top;
	float	bottom;

	float	fov;
	float	yaw;
	float	pitch;
	float	roll;
	float	speedx;
	float	speedy;
	float	speedz;

};

