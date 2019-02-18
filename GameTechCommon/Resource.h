#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

class Resource:ScriptObject
{
public:
	Resource(GameObject * gameObject);
	Resource(GameObject* gameObject, InputManager *im);
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


