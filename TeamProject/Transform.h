#pragma once
#include "../Common/Matrix4.h"
#include "../Common/Vector3.h"
#include "../Common/Quaternion.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"

#include <vector>

using std::vector;

using namespace NCL::Maths;

namespace NCL {
	namespace CSC8503 {
		class PhysicsObject;
		class GameObject;
		
		class Transform
		{
		public:
			Transform();
			Transform(const Vector3& position, Transform* parent = nullptr);
			~Transform();

			void SetWorldPosition(const Vector3& worldPos);
			void SetLocalPosition(const Vector3& localPos);

			void SetWorldScale(const Vector3& worldScale);
			void SetLocalScale(const Vector3& localScale);

			Transform* GetParent() const {
				return parent;
			}

			void SetParent(Transform* newParent) {
				parent = newParent;
			}

			Matrix4 GetWorldMatrix() const {
				return worldMatrix;
			}

			Matrix4 GetLocalMatrix() const {
				return localMatrix;
			}

			Vector3 GetWorldPosition() const {
				return worldMatrix.GetPositionVector();
			}

			Vector3 GetLocalPosition() const {
				return localPosition;
			}

			Vector3 GetLocalScale() const {
				return localScale;
			}

			Quaternion GetLocalOrientation() const {
				return localOrientation;
			}

			void SetLocalOrientation(const Quaternion& newOr) {
				localOrientation = newOr;
			}

			Quaternion GetWorldOrientation() const {
				return worldOrientation;
			}

			Matrix3 GetInverseWorldOrientationMat() const {
				return worldOrientation.Conjugate().ToMatrix3();
			}

			void AddChild(Transform* transform)
			{
				children.push_back(transform);
			}

			void RemoveChild(Transform * transform)
			{
				std::remove(children.begin(), children.end(), transform);
			}

			void GetChildTransformIterators(std::vector<Transform*>::const_iterator&b, std::vector<Transform*>::const_iterator&e) const
			{
				b = children.begin();
				e = children.end();
			}


			void UpdateMatrices();
			vector<Transform*> GetChildrenList();

			void ForceUpdateWorldPosition(Vector3 pos);
			void ForceUpdateBulletWorldTransform(btTransform &temp);
			void ForceUpdateLocalRotation(Quaternion qt);

			void ForceUpdateWorldPositionWithTransform(Vector3 pos);
			void ForceUpdateLocalPositionWithTransform(Vector3 pos);
			void ForceUpdateLocalRotationWithTransform(Quaternion qt);

			void ForceUpdateScale(Vector3 scale);
			void ForceUpdateScaleWithTransform(Vector3 scale);

			void SetGameObject(GameObject * obj);
			GameObject * GetGameObject();

		protected:
			Matrix4		localMatrix;
			Matrix4		worldMatrix;

			Vector3		localPosition;
			Vector3		localScale;
			Quaternion	localOrientation;
			Quaternion  worldOrientation;

			Transform*	parent;
			GameObject * gameObject;

			vector<Transform*> children;
		};
	}
}
