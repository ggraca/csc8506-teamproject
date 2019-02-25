#include "ScriptObject.h"


ScriptObject::ScriptObject()
{
	gameObject = nullptr;

}


ScriptObject::ScriptObject(GameObject * go)
{
	this->gameObject = go;

}

ScriptObject::~ScriptObject()
{
	//don"t delete gameobject as it may still meant to live after script is detached
}

void ScriptObject::Awake()
{
}

void ScriptObject::Start()
{
}

void ScriptObject::Update(float dt)
{
}

void ScriptObject::LateUpdate(float dt)
{
}

void ScriptObject::OnCollisionBegin(GameObject * otherObject)
{
}

void ScriptObject::OnCollisionEnd(GameObject * otherObject)
{
}
