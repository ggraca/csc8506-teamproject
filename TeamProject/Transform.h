#pragma once
#include "../Common/Matrix4.h"
#include "../Common/Vector3.h"
#include "../Common/Quaternion.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"

#include <vector>

using std::vector;

namespace NCL {
	namespace CSC8503 {
		class PhysicsObject;
		class GameObject;
		
		class Transform
		{
		public:
			Transform();
			Transform(const  NCL::Maths::Vec3& position, Transform* parent = nullptr);
			~Transform();

			void SetWorldPosition(const NCL::Maths::Vec3& worldPos);
			void SetLocalPosition(const  NCL::Maths::Vec3& localPos);

			void SetWorldScale(const  NCL::Maths::Vec3& worldScale);
			void SetLocalScale(const  NCL::Maths::Vec3& localScale);

			Transform* GetParent() const {
				return parent;
			}

			void SetParent(Transform* newParent) {
				parent = newParent;
			}

			NCL::Maths::Mat4 GetWorldMatrix() const {
				return worldMatrix;
			}

			NCL::Maths::Mat4 GetLocalMatrix() const {
				return localMatrix;
			}

			NCL::Maths::Vec3 GetWorldPosition() const {
				return worldMatrix.GetPositionVector();
			}

			NCL::Maths::Vec3 GetLocalPosition() const {
				return localPosition;
			}

			NCL::Maths::Vec3 GetLocalScale() const {
				return localScale;
			}

			NCL::Maths::Quaternion GetLocalOrientation() const {
				return localOrientation;
			}

			void SetLocalOrientation(const  NCL::Maths::Quaternion& newOr) {
				localOrientation = newOr;
			}

			NCL::Maths::Quaternion GetWorldOrientation() const {
				return worldOrientation;
			}

			NCL::Maths::Matrix3 GetInverseWorldOrientationMat() const {
				return worldOrientation.Conjugate().ToMatrix3();
			}

			void AddChild(Transform* transform)
			{
				children.push_back(transform);
			}

			void RemoveChild(Transform * transform)
			{
				int count = 0;
				for (auto&i : children)
				{
					if (i->gameObject == transform->GetGameObject())
					{
						children.erase(children.begin() + count);
					}
					count++;
				}
			}

			void GetChildTransformIterators(std::vector<Transform*>::const_iterator&b, std::vector<Transform*>::const_iterator&e) const
			{
				b = children.begin();
				e = children.end();
			}


			void UpdateMatrices();
			vector<Transform*> GetChildrenList();

			void ForceUpdateWorldPosition(NCL::Maths::Vec3 pos);
			void ForceUpdateBulletWorldTransform(btTransform &temp);
			void ForceUpdateLocalRotation(NCL::Maths::Quaternion qt);

			void ForceUpdateWorldPositionWithTransform(NCL::Maths::Vec3 pos);
			void ForceUpdateLocalPositionWithTransform(NCL::Maths::Vec3 pos);
			void ForceUpdateLocalRotationWithTransform(NCL::Maths::Quaternion qt);

			void ForceUpdateScale(NCL::Maths::Vec3 scale);
			void ForceUpdateScaleWithTransform(NCL::Maths::Vec3 scale);

			void SetGameObject(GameObject * obj);
			GameObject* GetGameObject();


		protected:
			NCL::Maths::Mat4		localMatrix;
			NCL::Maths::Mat4		worldMatrix;

			NCL::Maths::Vec3		localPosition;
			NCL::Maths::Vec3		localScale;
			NCL::Maths::Quaternion	localOrientation;
			NCL::Maths::Quaternion  worldOrientation;

			Transform*	parent = nullptr;
			GameObject * gameObject;

			vector<Transform*> children;
		};
	}
}
