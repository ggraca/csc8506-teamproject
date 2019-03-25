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

PlayerInput* ScriptObject::GetPlayerInput()
{
	GameWorld* gw = GameObject::gameWorld;
	if (!gw) return nullptr;

	NetworkServer* ns = dynamic_cast<NetworkServer*>(gw->GetNetwork());
	if (!ns) return nullptr;

	PlayerInput* ps = ns->FindPlayer(gameObject);
	if (!ps) return nullptr;

	return ps;
}

InputContainer ScriptObject::GetKeysDown() 
{
	PlayerInput* ps = GetPlayerInput();
	if (!ps) return InputContainer();

	return ps->keysDown;
}

InputContainer ScriptObject::GetKeysPressed() 
{
	PlayerInput* ps = GetPlayerInput();
	if (!ps) return InputContainer();

	return ps->keysPressed;
}

Quaternion ScriptObject::GetCameraRotation() {
	PlayerInput* ps = GetPlayerInput();
	if (!ps) return Quaternion();

	return ps->cameraRotation;
}

Vector3 ScriptObject::GetCameraPosition() {
	PlayerInput* ps = GetPlayerInput();
	if (!ps) return Vector3();

	return ps->cameraPosition;
}

int ScriptObject::GetNetworkId()
{
	PlayerInput* ps = GetPlayerInput();
	if (!ps) return -2;

	return ps->peerId;
}