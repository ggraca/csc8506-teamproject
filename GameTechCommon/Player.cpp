#include "Player.h"
#include "../TeamProject/InputManager.h"


Player::Player(GameObject * obj) : ScriptObject(obj)
{

}

Player::Player(GameObject * obj, InputManager * im):ScriptObject(obj,im)
{
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
