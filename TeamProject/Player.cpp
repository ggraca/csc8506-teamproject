#include "Player.h"
#include "InputManager.h"
#include "GunControl.h"
#include "HammerControl.h"
#include "ShieldControl.h"
#include "BigGunControl.h"

Player::Player(GameObject* obj) : ScriptObject(obj)
{
	ResetPlayer();
}

void Player::Awake()
{
}

void Player::Start()
{
}

void Player::Update(float dt)
{
	PlayerMovement(dt);
	CheckGunControls();
	CheckHammerControls();
	CheckShieldControls();
	CheckBigGunControls(dt);
}

void Player::CheckBigGunControls(float dt)
{
	if (timeCounter >= 0.0f) { timeCounter += dt;}
	if (!isHammerActive && !isShieldActive && !isGunActive && InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::HIT))
	{
		isBigGunActive = true;
		timeCounter = dt;

		gameObject->GetComponent<BigGunControl*>()->ActivateGun();
	}

	if (isBigGunActive && !InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::HIT))
	{
		if (resourceCount > 3)
		{
			gameObject->GetComponent<BigGunControl*>()->Fire(timeCounter);	
		}
		gameObject->GetComponent<BigGunControl*>()->DeactivateGun();
		isBigGunActive = false;
		timeCounter = -1;
	}
}

void Player::CheckShieldControls()
{
	if (!isGunActive && !isHammerActive && !isBigGunActive && InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::TOGGLE_SHIELD))
	{
		isShieldActive = true;

		gameObject->GetComponent<ShieldControl*>()->ActivateShield();
	}

	if (isShieldActive && !InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::TOGGLE_SHIELD))
	{
		isShieldActive = false;

		gameObject->GetComponent<ShieldControl*>()->DeactivateShield();
	}
}

void Player::CheckHammerControls()
{
	if (!isGunActive && !isShieldActive && !isBigGunActive && InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::TOGGLE_HAMMER))
	{
		isHammerActive = !isHammerActive;

		if (isHammerActive) { gameObject->GetComponent<HammerControl*>()->ActivateHammer(); }
		else { gameObject->GetComponent<HammerControl*>()->DeactivateHammer(); }
	}

	if (isHammerActive && InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::HIT))
	{
		if (resourceCount > 0) { gameObject->GetComponent<HammerControl*>()->HammerHit(); }
	}
}

void Player::CheckGunControls()
{
	if (!isHammerActive && !isShieldActive && !isBigGunActive && InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::TOGGLE_GUN))
	{
		isGunActive = !isGunActive;

		if (isGunActive) { gameObject->GetComponent<GunControl*>()->ActivateGun(); }
		else { gameObject->GetComponent<GunControl*>()->DeactivateGun(); }
	}

	if (isGunActive && InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::HIT))
	{
		if (resourceCount > 0) { gameObject->GetComponent<GunControl*>()->Fire(); }
	}
}

void Player::PlayerMovement(float dt)
{
	Vector3 playerPos = gameObject->GetTransform().GetWorldPosition();
	Vector3 cameraPos = GameObject::GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition();
	Vector3 up = Vector3(0, 1, 0);
	Vector3 forward = (Vector3(playerPos.x, 0, playerPos.z) - Vector3(cameraPos.x, 0, cameraPos.z)).Normalised();
	if (!GameObject::GetMainCamera()->GetComponent<CameraControl*>()->GetCameraType()) { forward *= -1; }
	Vector3 left = Vector3::Cross(up, forward).Normalised();

	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::FORWARD))
	{
		if (InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::DODGE))
		{
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity( btVector3(forward.x, forward.y, forward.z)*dodgeAmount);
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setDamping(0.5, 0);
		}
		else 
		{
			playerPos += forward * movementSpeed * dt;
			gameObject->GetTransform().SetWorldPosition(playerPos);
		}
		
	}

	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::BACKWARD))
	{
		if (InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::DODGE))
		{
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(-1 * btVector3(forward.x, forward.y, forward.z)*dodgeAmount);
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setDamping(0.5, 0);
		}
		else
		{
			playerPos -= forward * movementSpeed * dt;
			gameObject->GetTransform().SetWorldPosition(playerPos);
		}
		
	}

	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::LEFT))
	{
		if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::DODGE))
		{
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(btVector3(left.x, left.y, left.z)*dodgeAmount);
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setDamping(0.5, 0);
		}
		else
		{
			playerPos += left * movementSpeed * dt;
			gameObject->GetTransform().SetWorldPosition(playerPos);
		}
		
	}

	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::RIGHT))
	{
		if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::DODGE))
		{
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(-1* btVector3(left.x, left.y, left.z)*dodgeAmount);
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setDamping(0.5, 0);
		}
		else
		{
			playerPos -= left * movementSpeed * dt;
			gameObject->GetTransform().SetWorldPosition(playerPos);
		}
	}

	if (InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::JUMP))
	{
		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(btVector3(0, 200, 0));
	}

	
}


void Player::LateUpdate(float dt)
{
	
}

void Player::OnCollisionBegin(GameObject * otherObject)
{
	if (!otherObject) { return; }

	if (otherObject->CompareTag(LayerAndTag::Tags::Resources))
	{
		otherObject->GetComponent<Resource*>()->Aquire(gameObject);
		UpdateResourceCount(1);
	}
}

void Player::OnCollisionEnd(GameObject * otherObject) {
}

int Player::GetResourceCount() const
{
	return resourceCount;
}

void Player::ResetPlayer()
{
	resourceCount = 0;
	movementSpeed = 200;
	jumpSpeed = 400;
	dodgeAmount = 500;
}

void Player::UpdateResourceCount(int amount)
{
	resourceCount += amount;

	if (resourceCount <= 0) resourceCount = 0;
}
