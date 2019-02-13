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
}

void Destructable::Update(float dt)
{
}

void Destructable::LateUpdate(float dt)
{
}

void Destructable::OnCollisionBegin(GameObject * otherObject)
{
}

void Destructable::OnCollisionEnd(GameObject * otherObject)
{
}
