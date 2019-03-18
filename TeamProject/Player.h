#pragma once
#include "GameObject.h"
#include "Resource.h"

using namespace NCL;
using namespace CSC8503;


class Player : virtual public ScriptObject
{
public:

	Player(GameObject * obj);
	virtual ~Player() {}
	void Awake() override;
	void Start() override;
	void Update(float dt) override;
	void CheckShieldControls();
	void CheckHammerControls();
	void CheckGunControls();
	void PlayerMovement(float dt);
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
	void UpdateResourceCount(int amount);
	int GetResourceCount() const;

protected:

	void ResetPlayer();

	int resourceCount;
	float movementSpeed;
	float jumpSpeed;
	bool isGunActive = false;
	bool isHammerActive = false;
	bool isShieldActive = false;
};

