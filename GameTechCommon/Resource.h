#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

class Resource:ScriptObject
{
public:
	Resource(GameObject * gameObject);
	~Resource();

	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
};

