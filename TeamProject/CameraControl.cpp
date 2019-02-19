#include "CameraControl.h"
#include "../TeamProject/InputManager.h"


CameraControl::CameraControl(GameObject * obj) : ScriptObject(obj)
{

}

CameraControl::CameraControl(GameObject * obj, InputManager * im) : ScriptObject(obj, im)
{
}

void CameraControl::Awake()
{
}

void CameraControl::Start()
{
}

void CameraControl::Update(float dt)
{
}

void CameraControl::LateUpdate(float dt)
{

}

void CameraControl::OnCollisionBegin(GameObject * otherObject)
{
}

void CameraControl::OnCollisionEnd(GameObject * otherObject)
{
}
