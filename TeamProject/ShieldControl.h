#pragma once
#include "ScriptObject.h"

class ShieldControl : public ScriptObject
{

public:
	ShieldControl(GameObject * obj);
	~ShieldControl();

	void Awake() override;
	void SetTarget(Transform * t);
	void Update(float dt) override;
	void ActivateShield();
	void DeactivateShield();
	void SetShield(GameObject * s);
	void SetShieldDummy(GameObject * s);
	void FollowTarget();
	void FormShield();
	void DeformShield();

protected:
	Transform * target = nullptr;
	GameObject * shield = nullptr;
	GameObject * shieldDummy = nullptr;
};

