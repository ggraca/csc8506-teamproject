#include "ResourcePrefab.h"



ResourcePrefab::ResourcePrefab()
{
	//auto resource = new CubePrefab(Vector3(100, 20, 100), Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 0), 0), Vector3(5, 5, 5), 100, 0.2f, 0.f);
	AddComponent<Resource*>((Component*)new Resource(this));
}


ResourcePrefab::~ResourcePrefab()
{
}
