#include "Player.h"
#include "InputManager.h"


Player::Player(GameObject* obj) : ScriptObject(obj)
{
	ResetPlayer();
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
}

void Player::UpdateResourceCount(int amount)
{
	resourceCount += amount;

	if (resourceCount <= 0) resourceCount = 0;
}
