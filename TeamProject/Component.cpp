#include "Component.h"

Component::Component()
{
	gameObject = nullptr;
}

Component::Component(GameObject * go)
{
	gameObject = go;
}

Component::~Component()
{
}

void Component::Update(float dt)
{
	std::cout << "Component Update" << std::endl;
}

void Component::SetGameObject(GameObject * go)
{
	gameObject = go;
}

GameObject * Component::GetGameObject() const
{
	return gameObject;
}
