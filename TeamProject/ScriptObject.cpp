#include "ScriptObject.h"
#include "NetworkServer.h"
#include "GameWorld.h"


ScriptObject::ScriptObject()
{
}

ScriptObject::ScriptObject(GameObject * go) : Component(go)
{

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

PlayerState* ScriptObject::GetPlayerInput() {
	GameWorld* gw = gameObject->gameWorld;
	if (!gw) return nullptr;

	NetworkServer* ns = dynamic_cast<NetworkServer*>(gw->GetNetwork());
	if (!ns) return nullptr;

	PlayerState* ps = ns->FindPlayer(gameObject);
	if (!ps) return nullptr;

	InputContainer* ic = ps->keysDown;
	return nullptr;
}

InputContainer ScriptObject::GetKeysDown() {
	PlayerState* ps = GetPlayerInput();
	if (!ps) return InputContainer();

	InputContainer* ic = ps->keysDown;
	if (!ic) return InputContainer();

	return *ic;
}

InputContainer ScriptObject::GetKeysPressed() {
	PlayerState* ps = GetPlayerInput();
	if (!ps) return InputContainer();

	InputContainer* ic = ps->keysDown;
	if (!ic) return InputContainer();

	return *ic;
}