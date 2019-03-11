#include "Player.h"
#include "InputManager.h"
#include "GunControl.h"

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
	if (InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::FIRE))
	{
		if(resourceCount > 0 )gameObject->GetComponent<GunControl*>()->Fire();
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
		playerPos += forward * movementSpeed * dt;
		gameObject->GetTransform().ForceUpdateWorldPositionWithTransform(playerPos);
	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::BACKWARD))
	{
		playerPos -= forward * movementSpeed * dt;
		gameObject->GetTransform().ForceUpdateWorldPositionWithTransform(playerPos);
	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::LEFT))
	{
		playerPos += left * movementSpeed * dt;
		gameObject->GetTransform().ForceUpdateWorldPositionWithTransform(playerPos);
	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::RIGHT))
	{
		playerPos -= left * movementSpeed * dt;
		gameObject->GetTransform().ForceUpdateWorldPositionWithTransform(playerPos);
	}
	if (InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::JUMP))
	{
		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(btVector3(0, 150, 0));
	}
}

void Player::LateUpdate(float dt)
{
	
}

void Player::OnCollisionBegin(GameObject * otherObject)
{
	if (!otherObject) { return; }
	cout << "Colliding with: " << otherObject->GetName() << endl;
	if (otherObject->CompareTag(LayerAndTag::Tags::Resources))
	{
		otherObject->GetComponent<Resource*>()->Aquire(gameObject);
		UpdateResourceCount(1);
	}
}

void Player::OnCollisionEnd(GameObject * otherObject)
{
	cout << "Stopped colliding with: " << otherObject->GetName() << endl;
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
}

void Player::UpdateResourceCount(int amount)
{
	resourceCount += amount;

	if (resourceCount <= 0) resourceCount = 0;
}
