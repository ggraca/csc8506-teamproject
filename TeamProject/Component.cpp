#include "Component.h"

Component::Component()
{
	gameObj = nullptr;
}

Component::Component(GameObject * go)
{
	gameObj = go;
}

Component::~Component()
{
}

void Component::Update(float dt)
{
}

void Component::SetGameObject(GameObject * go)
{
	gameObj = go;
}

GameObject * Component::GetGameObject() const
{
	return gameObj;
}
