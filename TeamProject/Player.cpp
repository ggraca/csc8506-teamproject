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
	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::FORWARD))
	{
		auto pos = gameObject->GetTransform().GetWorldPosition();
		pos -= Vector3(0, 0, 1) * movementSpeed * dt;
		gameObject->GetTransform().SetWorldPosition(pos);
	}
	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::BACKWARD))
	{
		auto pos = gameObject->GetTransform().GetWorldPosition();
		pos += Vector3(0, 0, 1) * movementSpeed * dt;
		gameObject->GetTransform().SetWorldPosition(pos);
	}
	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::LEFT))
	{
		auto pos = gameObject->GetTransform().GetWorldPosition();
		pos -= Vector3(1, 0, 0) * movementSpeed * dt;
		gameObject->GetTransform().SetWorldPosition(pos);
	}
	if (ExampleScene::inputManager->IsButtonDown(InputManager::ActionButton::RIGHT))
	{
		auto pos = gameObject->GetTransform().GetWorldPosition();
		pos += Vector3(1, 0, 0) * movementSpeed * dt;
		gameObject->GetTransform().SetWorldPosition(pos);
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
	movementSpeed = 20;
}

void Player::UpdateResourceCount(int amount)
{
	resourceCount += amount;

	if (resourceCount <= 0) resourceCount = 0;
	cout << "Resource Count is: " << resourceCount << endl;
}
