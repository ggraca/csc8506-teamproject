#pragma once
#include "Matrix4.h"
#include "Vector3.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

namespace NCL {
	enum CameraType {
		Orthographic,
		Perspective
	};

	class Camera {
	public:
		Camera(void) {
			pitch		= 0.0f;
			yaw			= 0.0f;
			roll = 0.0f;
			speedx = 1.0f;
			speedy = 1.0f;
			speedz = 1.0f;

			this->fov	= 45.0f;
			this->nearPlane = 1.0f;
			this->farPlane	= 100.0f;

			this->camType = CameraType::Perspective;
		};

		Camera(float pitch, float yaw, const  NCL::Maths::Vector3& position) {
			this->pitch		= pitch;
			this->yaw		= yaw;
			this->position	= position;
			roll = 0.0f;
			speedx = 1.0f;
			speedy = 1.0f;
			speedz = 1.0f;

			this->fov		= 45.0f;
			this->nearPlane = 1.0f;
			this->farPlane	= 100.0f;

			this->camType	= CameraType::Perspective;
		}

		~Camera(void) {};

		void UpdateCamera(float dt);

		float GetFieldOfVision() const {
			return fov;
		}

		float GetNearPlane() const {
			return nearPlane;
		}

		float GetFarPlane() const {
			return farPlane;
		}

		void SetNearPlane(float val) {
			nearPlane = val;
		}
		
		void SetFarPlane(float val) {
			farPlane = val;
		}

		//Builds a view matrix for the current camera variables, suitable for sending straight
		//to a vertex shader (i.e it's already an 'inverse camera matrix').
		NCL::Maths::Matrix4 BuildViewMatrix() const;

		NCL::Maths::Matrix4 BuildProjectionMatrix(float currentAspect = 1.0f) const;

		//Gets position in world space
		NCL::Maths::Vector3 GetPosition() const { return position; }
		//Sets position in world space
		void	SetPosition(const NCL::Maths::Vector3& val) { position = val; }

		//Gets yaw, in degrees
		float	GetYaw()   const { return yaw; }
		//Sets yaw, in degrees
		void	SetYaw(float y) { yaw = y; }

		//Gets pitch, in degrees
		float	GetPitch() const { return pitch; }
		//Sets pitch, in degrees
		void	SetPitch(float p) { pitch = p; }

		static Camera BuildPerspectiveCamera(const  NCL::Maths::Vector3& pos, float pitch, float yaw, float fov, float near, float far);
		static Camera BuildOrthoCamera(const  NCL::Maths::Vector3& pos, float pitch, float yaw, float left, float right, float top, float bottom, float near, float far);
	protected:
		CameraType camType;

		float	nearPlane;
		float	farPlane;
		float	left;
		float	right;
		float	top;
		float	bottom;

		float	fov;
		float	yaw;
		float	pitch;
		float	roll;
		float	speedx;
		float	speedy;
		float	speedz;
		NCL::Maths::Vector3 position;
	};
}