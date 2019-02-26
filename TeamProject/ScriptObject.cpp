#include "ScriptObject.h"


ScriptObject::ScriptObject()
{
	//gameObject = nullptr;
}


ScriptObject::ScriptObject(GameObject * go):Component(go)
{
	//this->gameObject = go;

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
	std::cout << "Script Update" << std::endl;
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
