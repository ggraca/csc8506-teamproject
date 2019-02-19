#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;


class CameraControl : virtual public ScriptObject
{
public:

	CameraControl(GameObject * obj);
	CameraControl(GameObject * obj, InputManager *im);
	virtual ~CameraControl() {}

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
};

