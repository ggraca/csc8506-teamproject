#include "PlayerMovement.h"
#include "InputManager.h"


void PlayerMovement::Update(float dt) {
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