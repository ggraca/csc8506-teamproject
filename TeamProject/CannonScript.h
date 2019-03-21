#pragma once
#include "ScriptObject.h"


class CannonScript : virtual public ScriptObject
{
public:
public:
	CannonScript(GameObject * gameObject);
	~CannonScript();

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void CheckIfDestroyed();
	void LateUpdate(float dt) override;
};

