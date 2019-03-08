#include "Transform.h"
#include "BulletPhysics.h"
using namespace NCL::CSC8503;

Transform::Transform()	{
	parent		= nullptr;
	localScale	= Vector3(1, 1, 1);
}

Transform::Transform(const Vector3& position, Transform* p) {
	parent = p;
	SetWorldPosition(position);
}

Transform::~Transform(){
}

void Transform::UpdateMatrices() {
	localMatrix =
		Matrix4::Translation(localPosition) *
		localOrientation.ToMatrix4() *
		Matrix4::Scale(localScale);

	if (parent) {
		worldMatrix			= parent->GetWorldMatrix() * localMatrix;
		worldOrientation	= parent->GetWorldOrientation() * localOrientation;
	}
	else {
		worldMatrix			= localMatrix;
		worldOrientation	= localOrientation;
	}
}

vector<Transform*> NCL::CSC8503::Transform::GetChildrenList()
{
	return children;
}

void Transform::SetWorldPosition(const Vector3& worldPos) {
	if (parent) {
		Vector3 parentPos = parent->GetWorldMatrix().GetPositionVector();
		Vector3 posDiff = parentPos - worldPos;

		localPosition = posDiff;
		localMatrix.SetPositionVector(posDiff);
	}
	else {
		localPosition = worldPos;
		worldMatrix.SetPositionVector(worldPos);
	}
}

void Transform::SetLocalPosition(const Vector3& localPos) {
	localPosition = localPos;
}

void Transform::SetWorldScale(const Vector3& worldScale) {
	if (parent) {

	}
	else {
		localScale = worldScale;
	}
}

void Transform::SetLocalScale(const Vector3& newScale) {
	localScale = newScale;
}

void Transform::ForceUpdateBulletWorldTransform(btTransform &temp)
{
	btRigidBody * body = gameObject->GetComponent<PhysicsObject*>()->GetRigidbody();

	if (!body) { return; }

	body->getMotionState()->setWorldTransform(temp);
	body->setWorldTransform(temp);
}

void Transform::ForceUpdateWorldPosition(Vector3 pos)
{
	btRigidBody * body = gameObject->GetComponent<PhysicsObject*>()->GetRigidbody();

	if (!body) { return; }

	btTransform temp;
	body->getMotionState()->getWorldTransform(temp);
	temp.setOrigin(btVector3(pos.x, pos.y, pos.z));
	ForceUpdateBulletWorldTransform(temp);
}

void Transform::ForceUpdateWorldPositionWithTransform(Vector3 pos)
{
	SetWorldPosition(pos);
	ForceUpdateWorldPosition(pos);
}

void Transform::ForceUpdateLocalPositionWithTransform(Vector3 pos)
{
	SetLocalPosition(pos);
	UpdateMatrices();
	ForceUpdateWorldPosition(GetWorldPosition());
}

void Transform::ForceUpdateLocalRotation(Quaternion qt)
{
	btRigidBody * body = gameObject->GetComponent<PhysicsObject*>()->GetRigidbody();

	if (!body) { return; }

	btTransform temp;
	body->getMotionState()->getWorldTransform(temp);
	temp.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
	ForceUpdateBulletWorldTransform(temp);
}

void Transform::ForceUpdateLocalRotationWithTransform(Quaternion qt)
{
	SetLocalOrientation(qt);
	UpdateMatrices();
	ForceUpdateLocalRotation(qt);
}

void Transform::SetGameObject(GameObject * obj)
{
	gameObject = obj;
}

void Transform::ForceUpdateScale(Vector3 scale)
{
	btRigidBody * body = gameObject->GetComponent<PhysicsObject*>()->GetRigidbody();

	if (!body) { return; }

	if (body->getCollisionShape()->getShapeType() == cube) {
		body->setCollisionShape(new btBoxShape(btVector3(scale.x, scale.y, scale.z)));
	}
	if (body->getCollisionShape()->getShapeType() == sphere) {
		body->setCollisionShape(new btSphereShape(scale.x));
	}
	if (body->getCollisionShape()->getShapeType() == cylinder) {
		body->setCollisionShape(new btCylinderShape(btVector3(btScalar(scale.x), btScalar(scale.y), btScalar(scale.z))));
	}
	if (body->getCollisionShape()->getShapeType() == cone) {
		body->setCollisionShape(new btConeShape(btScalar(scale.x), btScalar(scale.y)));
	}
}

void Transform::ForceUpdateScaleWithTransform(Vector3 scale)
{
	SetLocalScale(scale);
	UpdateMatrices();
	ForceUpdateScale(scale);
}