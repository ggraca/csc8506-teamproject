#include "Destructable.h"
#include "../TeamProject/InputManager.h"

Destructable::Destructable(GameObject * gameObject):ScriptObject(gameObject)
{
	
}

Destructable::Destructable(GameObject * gameObject, InputManager * im):ScriptObject(gameObject,im)
{
}

Destructable::~Destructable()
{
	
}


void Destructable::Awake()
{
}

void Destructable::Start()
{
	if (gameObject->CompareTag(LayerAndTag::SmallWall))
	{
		objectHealth = 10;
	}
	if (gameObject->CompareTag(LayerAndTag::BigWall))
	{
		objectHealth = 20;
	}
	if (gameObject->CompareTag(LayerAndTag::Tower))
	{
		objectHealth = 15;
	}
	if (gameObject->CompareTag(LayerAndTag::Turret))
	{
		objectHealth = 5;
	}

	
}

void Destructable::Update(float dt)
{
	//TODO DESTROY OBJECT WHEN HEALTH IS 0 AND CREATE RESOURCES IN ITS POSITION

}

void Destructable::LateUpdate(float dt)
{
}

void Destructable::OnCollisionBegin(GameObject * otherObject)
{
	if (otherObject->CompareTag(LayerAndTag::Resource))
	{
		objectHealth--;
	}
}

void Destructable::OnCollisionEnd(GameObject * otherObject)
{
}
