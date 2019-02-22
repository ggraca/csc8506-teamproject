#include "Destructable.h"
#include "InputManager.h"

Destructable::Destructable(GameObject * gameObject):ScriptObject(gameObject)
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
