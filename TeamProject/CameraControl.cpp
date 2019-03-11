#include "CameraControl.h"
#include "InputManager.h"


CameraControl::CameraControl(GameObject * obj) : ScriptObject(obj)
{
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
	speedx = 1.0f;
	speedy = 1.0f;
	speedz = 1.0f;

	this->fov = 45.0f;
	this->nearPlane = 3.0f;
	this->farPlane = 4200.0f;
	this->pitch = 0;
	this->yaw = 0;
	gameObject->GetTransform().SetWorldPosition(Vector3(0, 10,0));
}

CameraControl::~CameraControl()
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
	FollowPlayer();
	UpdateCamera();
	RotatePlayer();
}

void CameraControl::UpdateCamera()
{
	roll -= 5.0f * (Window::GetMouse()->GetWheelMovement());
	pitch += (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	roll = min(roll, 90.0f);
	roll = max(roll, -90.0f);

	if (isTPS)
	{
		pitch = min(pitch, 50.0f);
		pitch = max(pitch, -25.0f);
	}
	else
	{
		pitch = min(pitch, 10.0f);
		pitch = max(pitch, -90.0f);
	}
	

	if (yaw < 0) 
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f) 
	{
		yaw -= 360.0f;
	}

	gameObject->GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(pitch, yaw, roll));
	gameObject->GetTransform().UpdateMatrices();

	//GameObject::Find("Cube")->GetTransform().SetLocalOrientation(Quaternion::EulerAnglesToQuaternion(0, yaw, 0));
	//GameObject::Find("Cube")->GetTransform().UpdateMatrices();
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

Matrix4 CameraControl::BuildViewMatrix() const 
{
	vector<Transform*> children = gameObject->GetTransform().GetChildrenList();
	
	if (children.size() == 0)
	{
		cout << "No camera child has been found" << endl;
		return	Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
			Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
			Matrix4::Translation(-gameObject->GetTransform().GetWorldPosition());
	}
	else
	{
		children[0]->UpdateMatrices();
		Vector3 childRot = children[0]->GetWorldOrientation().ToEuler();
		
		return	Matrix4::Rotation(-childRot.x, Vector3(1, 0, 0)) *
			Matrix4::Rotation(-childRot.y, Vector3(0, 1, 0)) *
			Matrix4::Translation(-children[0]->GetWorldPosition());
	}
};

Matrix4 CameraControl::BuildProjectionMatrix(float currentAspect) const
{
	//if (camType == CameraType::Orthographic) {
	//	return Matrix4::Orthographic(nearPlane, farPlane, right, left, top, bottom);
	//}
	//else if (camType == CameraType::Perspective) {
	return Matrix4::Perspective(nearPlane, farPlane, currentAspect, fov);
	//}
}

void CameraControl::SetPlayer(GameObject * obj)
{
	this->player = obj;
}

void CameraControl::FollowPlayer()
{
	if (!player) { return; }

	gameObject->GetTransform().SetWorldPosition(player->GetTransform().GetWorldPosition());
}

void CameraControl::RotatePlayer()
{
	if (!player) { return; }

	Vector3 playerRot = player->GetTransform().GetLocalOrientation().ToEuler();
	playerRot.y = gameObject->GetTransform().GetLocalOrientation().ToEuler().y;
	player->GetTransform().ForceUpdateLocalRotation(Quaternion::EulerAnglesToQuaternion(playerRot.x, playerRot.y, 0));
}

void CameraControl::SetCameraType(bool isTPSType)
{
	isTPS = isTPSType;
}

bool CameraControl::GetCameraType() const
{
	return isTPS;
}
