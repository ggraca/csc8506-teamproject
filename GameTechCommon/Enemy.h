#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

class Enemy : virtual public ScriptObject
{
public:

	Enemy(GameObject * obj);
	virtual ~Enemy() {}

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
};

