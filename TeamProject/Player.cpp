#include "Player.h"
#include "InputManager.h"


Player::Player(GameObject* obj) : ScriptObject(obj)
{
	ResetPlayer();
	keyDown = false;
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
		playerPos += forward *movementSpeed * dt;
		/*gameObject->GetTransform().SetWorldPosition(playerPos);
		gameObject->GetPhysicsObject()->GetRigidbody()->getWorldTransform().setOrigin(btVector3(playerPos.x, playerPos.y, playerPos.z));*/

		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(movementSpeed * btVector3(forward.x, forward.y, forward.z));
		keyDown = true;
		reset = false;
	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::BACKWARD))
	{
		playerPos -= forward * movementSpeed * dt;
		/*gameObject->GetTransform().SetWorldPosition(playerPos);
		gameObject->GetPhysicsObject()->GetRigidbody()->getWorldTransform().setOrigin(btVector3(playerPos.x, playerPos.y, playerPos.z));*/

		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(-movementSpeed * btVector3(forward.x, forward.y, forward.z));
		keyDown = true;
		reset = false;
	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::LEFT))
	{
		playerPos += left * movementSpeed * dt;
		/*gameObject->GetTransform().SetWorldPosition(playerPos);
		gameObject->GetPhysicsObject()->GetRigidbody()->getWorldTransform().setOrigin(btVector3(playerPos.x, playerPos.y, playerPos.z));*/

		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(movementSpeed * btVector3(left.x, left.y, left.z));
		keyDown = true;
		reset = false;
	}
	if (InputManager::GetInstance().IsButtonDown(InputManager::ActionButton::RIGHT))
	{
		playerPos -= left * movementSpeed * dt;
		/*gameObject->GetTransform().SetWorldPosition(playerPos);
		gameObject->GetPhysicsObject()->GetRigidbody()->getWorldTransform().setOrigin(btVector3(playerPos.x, playerPos.y, playerPos.z));*/

		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(-movementSpeed * btVector3(left.x, left.y, left.z));
		keyDown = true;
		reset = false;
	}
	if (InputManager::GetInstance().IsButtonPressed(InputManager::ActionButton::JUMP))
	{
		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->applyImpulse(btVector3(0, 2000, 0), btVector3(0, 0, 0));
		//gameObject->GetPhysicsObject()->ApplyForce(Vector3(0, 1000, 0), Vector3(0, 0, 0));
	}
	if (!keyDown && !reset) {
		gameObject->GetComponent<PhysicsObject*>()->GetRigidbody()->setLinearVelocity(btVector3(0, 0, 0));
		reset = true;
	}
	keyDown = false;
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
