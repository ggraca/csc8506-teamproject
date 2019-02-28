#pragma once
#include "GameObject.h"
#include "ExampleScene.h"

using namespace NCL;
using namespace CSC8503;

class Resource : virtual public ScriptObject
{
public:
	Resource();
	Resource(GameObject * obj);
	virtual ~Resource();

	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void FollowTarget(float &dt);
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
	void Aquire(GameObject * obj);
	void Reset();
	void SetTarget(GameObject * t);
	GameObject  * GetTarget() const;
	
	

private:
	float minDistance;
	float moveSpeed;
	GameObject  * target;
};


