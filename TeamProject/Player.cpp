#include "Player.h"
#include "InputManager.h"
#include "GunControl.h"
#include "HammerControl.h"
#include "ShieldControl.h"
#include "AirStrikeControl.h"
#include "CameraControl.h"
#include "PhysicsObject.h"
#include "BigGunControl.h"
#include "GameWorld.h"


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
	if (!dynamic_cast<NetworkServer*>(GameObject::gameWorld->GetNetwork())) return;

	keysDown = GetKeysDown();
	keysPressed = GetKeysPressed();
	cameraRotation = GetCameraRotation();
	cameraPosition = GetCameraPosition();

	PlayerRotation();
	PlayerMovement(dt);
	CheckGunControls();
	CheckHammerControls();
	CheckShieldControls();
	CheckAirStrikeControls();
	CheckBigGunControls(dt);
}

void Player::CheckAirStrikeControls()
{
	if (!isHammerActive && !isShieldActive && resourceCount >= 5 && keysPressed.inputs[InputManager::ActionButton::CALL_AIR_STRIKE])
	{
		gameObject->GetComponent<AirStrikeControl*>()->LaunchAirStrike();
	}
}

void Player::CheckBigGunControls(float dt)
{
	if (timeCounter >= 0.0f) { timeCounter += dt;}
	if (!isHammerActive && !isShieldActive && !isGunActive && keysPressed.inputs[InputManager::ActionButton::HIT])
	{
		isBigGunActive = true;
		timeCounter = dt;

		gameObject->GetComponent<BigGunControl*>()->ActivateGun();
	}

	if (isBigGunActive && !keysDown.inputs[InputManager::ActionButton::HIT])
	{
		if (resourceCount >= 3)
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
	if (!isGunActive && !isHammerActive && !isBigGunActive && keysPressed.inputs[InputManager::ActionButton::TOGGLE_SHIELD])
	{
		isShieldActive = true;

		gameObject->GetComponent<ShieldControl*>()->ActivateShield();
	}

	if (isShieldActive && !keysDown.inputs[InputManager::ActionButton::TOGGLE_SHIELD])
	{
		isShieldActive = false;

		gameObject->GetComponent<ShieldControl*>()->DeactivateShield();
	}
}

void Player::CheckHammerControls()
{
	if (!isGunActive && !isShieldActive && !isBigGunActive && keysPressed.inputs[InputManager::ActionButton::TOGGLE_HAMMER])
	{
		isHammerActive = !isHammerActive;

		if (isHammerActive) { gameObject->GetComponent<HammerControl*>()->ActivateHammer(); }
		else { gameObject->GetComponent<HammerControl*>()->DeactivateHammer(); }
	}

	if (isHammerActive && keysPressed.inputs[InputManager::ActionButton::HIT])
	{
		if (resourceCount > 0) { gameObject->GetComponent<HammerControl*>()->HammerHit(); }
	}
}

void Player::CheckGunControls()
{
	if (!isHammerActive && !isShieldActive && !isBigGunActive && keysPressed.inputs[InputManager::ActionButton::TOGGLE_GUN])
	{
		isGunActive = !isGunActive;

		if (isGunActive) { gameObject->GetComponent<GunControl*>()->ActivateGun(); }
		else { gameObject->GetComponent<GunControl*>()->DeactivateGun(); }
	}

	if (isGunActive && keysPressed.inputs[InputManager::ActionButton::HIT])
	{
		if (resourceCount > 0) { gameObject->GetComponent<GunControl*>()->Fire(); }
	}
}

void Player::PlayerRotation()
{
	Vector3 playerRot = gameObject->GetTransform().GetLocalOrientation().ToEuler();
	playerRot.y = cameraRotation.ToEuler().y;
	gameObject->GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(0, playerRot.y + 180, 0));
}

void Player::PlayerMovement(float dt)
{
	Vector3 playerPos = gameObject->GetTransform().GetWorldPosition();
	Vector3 up = Vector3(0, 1, 0);
	Vector3 forward = (Vector3(playerPos.x, 0, playerPos.z) - Vector3(cameraPosition.x, 0, cameraPosition.z)).Normalised();
	Vector3 left = Vector3::Cross(up, forward).Normalised();

	if (keysDown.inputs[InputManager::ActionButton::FORWARD])
	{
		if (keysPressed.inputs[InputManager::ActionButton::DODGE])
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

	if (keysDown.inputs[InputManager::ActionButton::BACKWARD])
	{
		if (keysPressed.inputs[InputManager::ActionButton::DODGE])
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

	if (keysDown.inputs[InputManager::ActionButton::LEFT])
	{
		if (keysDown.inputs[InputManager::ActionButton::DODGE])
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

	if (keysDown.inputs[InputManager::ActionButton::RIGHT])
	{
		if (keysDown.inputs[InputManager::ActionButton::DODGE])
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

	if (!isJumping && keysPressed.inputs[InputManager::ActionButton::JUMP])
	{
		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(btVector3(0, 200, 0));
		isJumping = true;
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

	else if (otherObject->CompareTag(LayerAndTag::Tags::Ground) || otherObject->CompareTag(LayerAndTag::Tags::Destructible))
	{
		isJumping = false;
	}
}

void Player::OnCollisionEnd(GameObject * otherObject)
{
}

int Player::GetResourceCount() const
{
	return resourceCount;
}

LayerAndTag::Tags Player::GetResourceTag() 
{
	int networkID = GetNetworkId();
	
	if (networkID == -2) { return LayerAndTag::Tags::ResourceParent; }
	return (LayerAndTag::Tags)((networkID + 1) + LayerAndTag::Occupied0);
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
