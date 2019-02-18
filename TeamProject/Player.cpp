#include "Player.h"
#include "../TeamProject/InputManager.h"


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
}

void Player::LateUpdate(float dt)
{
}

void Player::OnCollisionBegin(GameObject * otherObject)
{
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
}

void Player::UpdateResourceCount(int amount)
{
	resourceCount += amount;

	if (resourceCount <= 0) resourceCount = 0;
}
