#include "ResourcePrefab.h"
#include "NetworkObject.h"


ResourcePrefab::ResourcePrefab()
{
	AddComponent<Resource*>((Component*)new Resource(this));
	AddComponent<DamageControl*>(new DamageControl(this));
	AddComponent<NetworkObject*>(new NetworkObject(this));
}

ResourcePrefab::ResourcePrefab(const Vector3 & position, const Quaternion & orient, Vector3 dimensions, float mass, float restitution, float friction):CubePrefab(position,orient,dimensions,mass,restitution,friction)
{
	AddComponent<Resource*>((Component*)new Resource(this));
	AddComponent<DamageControl*>(new DamageControl(this));
	AddComponent<NetworkObject*>(new NetworkObject(this));
	
}

ResourcePrefab::~ResourcePrefab()
{
}
