#pragma once
#include "ScriptObject.h"

using namespace NCL;
using namespace CSC8503;


class Destructable: virtual public ScriptObject
{
public:
	Destructable(GameObject * gameObject);
	~Destructable();

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
};

