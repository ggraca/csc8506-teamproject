#include "Component.h"



Component::Component()
{
}


Component::Component(GameObject * go)
{
	gameObject = go;
}

Component::~Component()
{
}

void Component::Update()
{
}
