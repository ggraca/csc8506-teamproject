#include "CannonScript.h"




CannonScript::CannonScript(GameObject * gameObject) : ScriptObject(gameObject)
{
}


CannonScript::~CannonScript()
{
}

void CannonScript::Awake()
{
	health = gameObject->GetComponent<HealthManager*>();
	// generate list of all players + distance

}

void CannonScript::Start()
{

}

void CannonScript::Update(float dt)
{
	CheckIfDestroyed();
	// check players in range check closers one and fire


}

void CannonScript::CheckIfDestroyed()
{

}

void CannonScript::LateUpdate(float dt)
{
}
