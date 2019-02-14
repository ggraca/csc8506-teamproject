#include "Enemy.h"
#include "../TeamProject/InputManager.h"

Enemy::Enemy(GameObject * obj) :ScriptObject(obj)
{

}

Enemy::Enemy(GameObject * obj, InputManager * im):ScriptObject(obj,im)
{
}

void Enemy::Awake()
{
}

void Enemy::Start()
{
}

void Enemy::Update(float dt)
{
}

void Enemy::LateUpdate(float dt)
{
}

void Enemy::OnCollisionBegin(GameObject * otherObject)
{
}

void Enemy::OnCollisionEnd(GameObject * otherObject)
{
}
