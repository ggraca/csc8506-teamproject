#include "StallPrefab.h"
#include "Destructible.h"

StallPrefab::StallPrefab(const Vector3& dimensions, const Vector3& position, const Quaternion& orient)
{
	string filename = "vws.obj";

	AddRenderObject(filename, this, dimensions, position, orient);
	AddComponent<PhysicsObject*>((Component*)new PhysicsObject(&GetTransform(), ShapeType::complexMesh, 0, 0, 0, filename));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Stall));
	SetTag(LayerAndTag::Tags::Ground);
	AddComponent<HealthManager*>(new HealthManager(this, 20));
	AddComponent<Destructible*>(new Destructible(this));
}

StallPrefab::~StallPrefab()
{
}

void StallPrefab::SetTransformDetails(NCL::Maths::Vector3 &dimensions, const NCL::Maths::Vector3 & position, const NCL::Maths::Quaternion & orient)
{
	GetTransform().SetWorldScale(dimensions);
	GetTransform().SetWorldPosition(position);
	GetTransform().SetLocalOrientation(orient);
}