#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;


class Destructable:ScriptObject
{
public:
	Destructable(GameObject * gameObject);
	Destructable(GameObject * gameObject,InputManager * im);
	~Destructable();

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;


private:
	int objectHealth;
	int playerDamage;
};

