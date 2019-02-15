#include "Resource.h"
#include "../TeamProject/InputManager.h"


Resource::Resource(GameObject * gameObject):ScriptObject(gameObject)
{
}

Resource::Resource(GameObject * gameObject, InputManager * im):ScriptObject(gameObject,im)
{
}

Resource::~Resource()
{
}


void Resource::Awake()
{
}

void Resource::Start()
{
	
}

void Resource::Update(float dt)
{
}

void Resource::LateUpdate(float dt)
{
}

void Resource::OnCollisionBegin(GameObject * otherObject)
{
}

void Resource::OnCollisionEnd(GameObject * otherObject)
{
}
