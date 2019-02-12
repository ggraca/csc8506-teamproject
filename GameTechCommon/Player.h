#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

class Player : virtual public ScriptObject
{
public:

	Player(GameObject * obj);
	virtual ~Player() {}

	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
};

