#pragma once
#include "ScriptObject.h"

class ShieldControl : public ScriptObject
{

public:
	ShieldControl(GameObject * obj);
	~ShieldControl();

	void SetTarget(Transform * t);
	void Update(float dt) override;
	void ActivateShield();
	void DeactivateShield();

protected:
	Transform * target = nullptr;
};

