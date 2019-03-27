#include "TowerPrefab.h"
#include "Destructible.h"

TowerPrefab::TowerPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "War_Tower.obj";

	AddRenderObject(filename, this, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Tower));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 400));
	AddComponent<Destructible*>(new Destructible(this));
}

TowerPrefab::~TowerPrefab()
{
}

void TowerPrefab::SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
}