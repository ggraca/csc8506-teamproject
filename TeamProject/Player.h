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
	void CheckAirStrikeControls();
	void CheckBigGunControls(float dt);
	void CheckShieldControls();
	void CheckHammerControls();
	void CheckGunControls();
	void PlayerRotation();
	void PlayerMovement(float dt);
	void LateUpdate(float dt) override;
	void OnCollisionBegin(GameObject* otherObject)override;
	void OnCollisionEnd(GameObject* otherObject)override;
	void LoseResource(int amount);
	void LoseResource(GameObject * resource);
	void TakeDamage(int amount);
	int GetResourceCount() const;
	int GetHP() const;
	LayerAndTag::Tags GetResourceTag() ;
	vector<GameObject*> GetResources() const;
	bool IsHammerActive();
	bool IsGunActive();
	bool IsBugGunActive();

protected:

	void ResetPlayer();

	int hp;
	float movementSpeed;
	float jumpSpeed;
	float dodgeAmount;
	bool isGunActive = false;
	bool isHammerActive = false;
	bool isShieldActive = false;
	bool isBigGunActive = false;
	bool isJumping = false;
	float timeCounter = -1;
	bool isDead = false;

	InputContainer keysDown;
	InputContainer keysPressed;
	Quaternion cameraRotation;
	Vector3 cameraPosition;

	vector<GameObject*> resources;
};

