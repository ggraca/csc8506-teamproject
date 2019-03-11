#pragma once
#include "GameObject.h"
#include "Resource.h"
#include "HealthManager.h"
#include "ExampleScene.h"
#include "ScriptObject.h"
#include "ResourcePrefab.h"

using namespace NCL;
using namespace CSC8503;


class Destructible: virtual public ScriptObject
{
public:
	Destructible(GameObject * gameObject);
	~Destructible();

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void CheckIfDestroyed();
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;


private:

	void GenerateResource();
	Vector3 resourceDimensions;
	HealthManager * health;
};

