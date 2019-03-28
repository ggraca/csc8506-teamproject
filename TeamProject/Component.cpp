#include "Component.h"
#include "GameObject.h"
Component::Component()
{
}

Component::Component(GameObject * go) : gameObject(go)
{

}

Component::~Component()
{

}

void Component::Update(float dt)
{
}

void Component::SetGameObject(GameObject * go)
{
	gameObject = go;
}

GameObject * Component::GetGameObject() const
{
	return gameObject;
}
