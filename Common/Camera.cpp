#include "Camera.h"
#include "Window.h"
#include <algorithm>

using namespace NCL;

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float dt) {
	roll -= 5.0f * (Window::GetMouse()->GetWheelMovement());
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	roll = min(roll, 90.0f);
	roll = max(roll, -90.0f);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch, 90.0f);
	pitch = max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	float frameSpeed = 100 * dt;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W) || Window::GetGamepad()->KeyDown(SCE_PAD_BUTTON_UP)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0)) * Vector3(0, 0, -1) * frameSpeed * speedz;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S) || Window::GetGamepad()->KeyDown(SCE_PAD_BUTTON_DOWN)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0)) * Vector3(0, 0, -1) * frameSpeed * speedz;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A) || Window::GetGamepad()->KeyDown(SCE_PAD_BUTTON_LEFT)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) *  Vector3(-1, 0, 0) * frameSpeed * speedx;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D) || Window::GetGamepad()->KeyDown(SCE_PAD_BUTTON_RIGHT)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * frameSpeed * speedx;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT) || Window::GetGamepad()->KeyDown(SCE_PAD_BUTTON_L1)) {
		position.y += frameSpeed * speedy;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE) || Window::GetGamepad()->KeyDown(SCE_PAD_BUTTON_CROSS)) {
		position.y -= frameSpeed * speedy;
	}
}


/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix() const {
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
};

Matrix4 Camera::BuildProjectionMatrix(float currentAspect) const {
	if (camType == CameraType::Orthographic) {
		return Matrix4::Orthographic(nearPlane, farPlane, right, left, top, bottom);
	}
	//else if (camType == CameraType::Perspective) {
		return Matrix4::Perspective(nearPlane, farPlane, currentAspect, fov);
	//}
}

Camera Camera::BuildPerspectiveCamera(const Vector3& pos, float pitch, float yaw, float fov, float nearPlane, float farPlane) {
	Camera c;
	c.camType	= CameraType::Perspective;
	c.position	= pos;
	c.pitch		= pitch;
	c.yaw		= yaw;
	c.nearPlane = nearPlane;
	c.farPlane  = farPlane;

	c.fov		= fov;

	return c;
}
Camera Camera::BuildOrthoCamera(const Vector3& pos, float pitch, float yaw, float left, float right, float top, float bottom, float nearPlane, float farPlane) {
	Camera c;
	c.camType	= CameraType::Orthographic;
	c.position	= pos;
	c.pitch		= pitch;
	c.yaw		= yaw;
	c.nearPlane = nearPlane;
	c.farPlane	= farPlane;

	c.left		= left;
	c.right		= right;
	c.top		= top;
	c.bottom	= bottom;

	return c;
}
