#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;


class Destructable:ScriptObject
{
public:
	Destructable(GameObject * gameObject);
	~Destructable();

	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
};

