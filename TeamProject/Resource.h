#pragma once
#include "GameObject.h"
#include "ExampleScene.h"

using namespace NCL;
using namespace CSC8503;

class Resource:ScriptObject
{
public:
	Resource(GameObject * gameObject);
	virtual ~Resource();

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
};

