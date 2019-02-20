#include "Player.h"


Player::Player(GameObject * obj) : ScriptObject(obj)
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
	if (!GameObject::GetMainCamera()->GetScript<CameraControl*>()->GetCameraType()) { forward *= -1; }
	Vector3 left = Vector3::Cross(up, forward).Normalised();

	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::FORWARD))
	{
		playerPos += forward *movementSpeed * dt;
		gameObject->GetTransform().SetWorldPosition(playerPos);
	}
	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::BACKWARD))
	{
		playerPos -= forward * movementSpeed * dt;
		gameObject->GetTransform().SetWorldPosition(playerPos);
	}
	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::LEFT))
	{
		playerPos += left * movementSpeed * dt;
		gameObject->GetTransform().SetWorldPosition(playerPos);
	}
	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::RIGHT))
	{
		playerPos -= left * movementSpeed * dt;
		gameObject->GetTransform().SetWorldPosition(playerPos);
	}
	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::JUMP))
	{
		gameObject->GetPhysicsObject()->AddForce(Vector3(0, 1, 0) * jumpSpeed);
	}
}

void Player::LateUpdate(float dt)
{
	
}

void Player::OnCollisionBegin(GameObject * otherObject)
{
	if (otherObject->CompareTag(LayerAndTag::Tags::Resources))
	{
		otherObject->GetScript<Resource*>()->Aquire(gameObject);
		UpdateResourceCount(1);
	}
}

void Player::OnCollisionEnd(GameObject * otherObject)
{
}

int Player::GetResourceCount() const
{
	return resourceCount;
}

void Player::ResetPlayer()
{
	resourceCount = 0;
	movementSpeed = 50;
	jumpSpeed = 40;
}

void Player::UpdateResourceCount(int amount)
{
	resourceCount += amount;

	if (resourceCount <= 0) resourceCount = 0;
	cout << "Resource Count is: " << resourceCount << endl;
}
