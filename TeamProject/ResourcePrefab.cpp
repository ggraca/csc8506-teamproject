#include "ResourcePrefab.h"
#include "NetworkObject.h"


ResourcePrefab::ResourcePrefab()
{
	AddComponent<Resource*>(new Resource(this));
	AddComponent<DamageControl*>(new DamageControl(this));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Resource));
}

ResourcePrefab::ResourcePrefab(const Vec3 & position, const Quaternion & orient, Vec3 dimensions, float mass, float restitution, float friction):CubePrefab(position,orient,dimensions,mass,restitution,friction)
{
	AddComponent<Resource*>(new Resource(this));
	AddComponent<DamageControl*>(new DamageControl(this));
	AddComponent<NetworkObject*>(new NetworkObject(this, NetworkObject::Resource));
	
}

ResourcePrefab::~ResourcePrefab()
{
}
