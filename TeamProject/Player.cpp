#include "Player.h"
#include "InputManager.h"


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
		if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::DODGE))
		{
			//playerPos += forward * movementSpeed * dt;
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(movementSpeed * btVector3(forward.x, forward.y, forward.z)*dodgeAmount*dt);
			
			//gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(movementSpeed * btVector3(0,0,0));
		}
		playerPos += forward * movementSpeed * dt;
		gameObject->GetTransform().ForceUpdateWorldPositionWithTransform(playerPos);

	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::BACKWARD))
	{
		if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::DODGE))
		{
			playerPos -= forward * movementSpeed * dt;
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(-movementSpeed * btVector3(forward.x, forward.y, forward.z)*dodgeAmount);
		}
		playerPos -= forward * movementSpeed * dt;
		gameObject->GetTransform().ForceUpdateWorldPositionWithTransform(playerPos);
	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::LEFT))
	{
		if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::DODGE))
		{
			playerPos += left * movementSpeed * dt;
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(movementSpeed * btVector3(left.x, left.y, left.z)*dodgeAmount);
		}
		playerPos += left * movementSpeed * dt;
		gameObject->GetTransform().ForceUpdateWorldPositionWithTransform(playerPos);
	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::RIGHT))
	{
		if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::DODGE))
		{
			playerPos -= left * movementSpeed * dt;
			gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(-movementSpeed * btVector3(left.x, left.y, left.z)*dodgeAmount);
		}
		playerPos -= left * movementSpeed * dt;
		gameObject->GetTransform().ForceUpdateWorldPositionWithTransform(playerPos);
	}
	if (InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::JUMP))
	{
		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->applyImpulse(btVector3(0, 2000, 0), btVector3(0, 0, 0));
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
	dodgeAmount = 3;
	dumpAmount = 0.5;
}

void Player::UpdateResourceCount(int amount)
{
	resourceCount += amount;

	if (resourceCount <= 0) resourceCount = 0;
}
